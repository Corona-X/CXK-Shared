#include <Kernel/XKAssemblyCode.h>
#include <Kernel/Shared/XKLegacy.h>

#if kCXArchIA && kCXBuildDev

// About the stacks for these functions... They don't have any!!
// That means that if there is an error in any of these routines,
// it will not be visible in the stack trace, but there routines
// don't call out anywhere external and they're generally too simple
// to really cause many errors so it should be alright...
//
// Also... they can be called even if there is no stack which has
// been setup. This is a pretty nice feature to have and it means
// that you can get pretty hacky with the calling of these routines.

.section kXKCodeSectionName
.align kXKNaturalAlignment

// Arguments:
//   %dx: scratch register
//
// Return Value:
//   failure: Jump to \fail
//   success: Continue execution
//
// Destroyed Registers:
//   %al
.macro XKSerialPortTestValue value, fail
    movb \value, %al                    // Load test value into accumulator
    outb %al, %dx                       // Send test value to scratch space
    inb %dx, %al                        // Read value in scratch space
    cmpb \value, %al                    // Check if value was saved correctly
    jne \fail                           // Jump to failure handler if it wasn't
.endm

// Arguments:
//   %esi: port base
//
// Return Value:
//   success: Continue execution
//   failure: Jump to \fail
//
// Destroyed Registers:
//   %al
//   %dx
.macro XKSerialPortTest fail
    leaw 7(%esi), %dx                   // Load address of scratch register (base + 7) to I/O register (%dx)
    XKSerialPortTestValue $0xC7, \fail  // Test with first value
    XKSerialPortTestValue $0x83, \fail  // test with second value
.endm

// Arguments:
//   \wr: temporary register
//   \times: number of times to loop
//
// Return Value:
//   ---
//
// Destroyed Registers:
//   \wr
.macro XKSerialDelay wr, times
    movw \times, \wr                    // Move counter to loop counter

    99:
        rep; nop                        // Do nothing (wait)
        decw \wr                        // Decrement loop counter
        jnz 99b                         // Again if we're not done

    // The reason why this loop should delay enough is that the CPU
    // *should* be tricked into throwing out its pipeline each time
    // the jump instruction is executed. I can't say for sure weather
    // or not this actually occurs as I haven't tested it, but I don't
    // think the branch prediction works well enough to see through my
    // tricks. (hopefully?)
.endm

// Arguments:
//   %di: port base
//
// Return Value (%ax):
//   success: port base
//   failure: kXKSerialPortError
//
// Destroyed Registers:
//   %al
//   %dx
//   %esi
XKDeclareFunction(XKSerialPortInit):
    movzwl %di, %esi                    // Save port address to %esi
    XKSerialPortTest 1f                 // Check if serial port exists; return otherwise
    callq XKSymbol(XKSerialPortReset)   // Reset Port (%di destroyed)
    leaw 4(%esi), %dx                   // Load modem control register (base + 4) to I/O port register (%dx)
    movb $0x02, %al                     // Load initial modem control register value
    outb %al, %dx                       // Send initial modem control register value to modem control port
    subw $4, %dx                        // Move %dx back to port base
    inb %dx, %al                        // Clear character buffer
    movw %dx, %ax                       // Load port intro return value
    ret                                 // Return port address (success)

    1:
        movw $kXKSerialPortError, %ax   // Load failure return value
        ret                             // Return error address

// Arguments:
//   %di: port base
//
// Return Value:
//   ---
//
// Destroyed Registers:
//   %al
//   %dx
//   %edi
XKDeclareFunction(XKSerialPortReset):
    movzwl %di, %edi                    // Save port address to %edi
    leaw 3(%edi), %dx                   // Load line control register (base + 3) to I/O port register (%dx)
    movb $0x80, %al                     // Load value to expose divisor registers into accumulator
    outb %al, %dx                       // Expose divisor registers
    leaw (%edi), %dx                    // Load port base into I/O register
    movb $1, %al                        // Load LSB of divisor into accumulator
    outb %al, %dx                       // Reset LSB of divisor to 1
    xorb %al, %al                       // Clear accumulator
    incw %dx                            // Increment I/O register to point to MSB of divisor
    outb %al, %dx                       // Reset MSB of divisor to 0
    leaw 3(%edi), %dx                   // Load line control register to I/O port register
    outb %al, %dx                       // Reset line control to 0
    incw %dx                            // Increment I/O register to point to modem control register
    outb %al, %dx                       // Reset modem control register to 0
    incw %dx                            // Increment I/O register to point to line status register
    movb $0x60, %al                     // Load default line status into the accumulator
    outb %al, %dx                       // Reset line status to 0x60
    incw %dx                            // Increment I/O register to point to modem status register
    inb %dx, %al                        // Load current value of modem status register
    andb $0xF0, %al                     // Unset low 4 bits of modem status register (in accumulator)
    outb %al, %dx                       // Reset modem status register
    xorb %al, %al                       // Clear accumulator
    leaw 1(%edi), %dx                   // Load interrupt enable register (base + 1) into I/O port register
    outb %al, %dx                       // Reset interrupt enable register to 0 (All interrupts disabled)
    incw %dx                            // Increment I/O register to point to FIFO control register
    outb %al, %dx                       // Reset FIFO control register to 0 (FIFO disabled)
    ret                                 // All done; return to parent

// Arguments:
//   %di: port
//   %sil: word length
//   %cl: parity
//   %dl: stop bits
//
// Return Value:
//   ---
//
// Destroyed Registers:
//   %al
//   %dx
XKDeclareFunction(XKSerialPortSetupLineControl):
    movb %cl, %al                       // Store parity bits in accumulator
    shlb $3, %al                        // Move parity bits to correct position in accumulator
    shlb $1, %dl                        // Move stop bit to correct position
    orb %dl, %al                        // Or stop bit into accumulator
    orb %sil, %al                       // Or word length bits into accumulator
    leaw 3(%edi), %dx                   // Load line control register (base + 3) into I/O register (%dx)
    outb %al, %dx                       // Send new line control value to line control register
    ret                                 // All done; return to parent

// Arguments:
//   %di: port
//   %si: divisor
//
// Return Value:
//   ---
//
// Destroyed Registers:
//   %ax
//   %cl
//   %dx
XKDeclareFunction(XKSerialPortSetBaudDivisor):
    leaw 3(%edi), %dx                   // Load line control register (base + 3) into I/O register (%dx)
    inb %dx, %al                        // Load current line control register into accumulator
    movb %al, %cl                       // Store origin line control value
    orb $0x80, %al                      // Enable bit which exposes divisor in accumulator
    outb %al, %dx                       // Output divisor access bit into line control register
    leaw (%edi), %dx                    // Load divisor LSB (base + 0) into I/O register
    movb %sil, %al                      // Load divisor LSB into accumulator
    outb %al, %dx                       // Output divisor LSB
    incw %dx                            // Increment I/O register to point to divisor MSB
    movw %si, %ax                       // Move full divisor to accumulator
    movb %ah, %al                       // Move MSB of divisor to the low 8 bits of the accumulator
    outb %al, %dx                       // Output divisor MSB
    leaw 3(%edi), %dx                   // Load line control register into I/O register
    movb %cl, %al                       // Load saved line control value back into accumulator
    outb %al, %dx                       // Output saved line control value
    ret                                 // All done; return to parent

// Arguments:
//   %di: port
//   %si: rate
//
// Return Value (%al):
//   success: 1
//   failure: 0
//
// Destroyed Registers:
//   %ax
//   %cl
//   %dx
//   %si
XKDeclareFunction(XKSerialPortSetBaudRate):
    testw %si, %si                      // Check to see if the rate is 0
    jz 1f                               // If it is, get out and avoid a division error

    movw $0xC200, %ax                   // Load clock rate LSB into the accumulator
    movw $0x1, %dx                      // Load clock rate MSB into extended accumulator
    divw %si                            // Divide by provided rate
    test %dx, %dx                       // Test for a remainder
    jnz 1f                              // Return if there was a remainder
    movw %ax, %si                       // Load divisor into second argument register
    callq XKSymbol(XKSerialPortSetBaudDivisor)  // Set the divisor
    movb $1, %al                        // Mark that we could calculate a whole divisor
    ret                                 // Return to parent (successfully)

    1:
        movb $0, %al                    // Signify that we failed to find a whole number divisor
        ret                             // An error occurred; return to parent

// Arguments:
//   %di: port
//   %sil: character
//   %cl: block
//
// Return Value (%al):
//   success: 1
//   failure: 0
//
// Destroyed Registers:
//   %al
//   %dx
XKDeclareFunction(XKSerialWriteCharacter):
    leaw 5(%edi), %dx                   //  Load line status register (base + 5) into I/O register (%dx)

    1:
        inb %dx, %al                    // Load line status register into the accumulator
        testb $0x20, %al                // Test to see if the serial chip is ready to transmit another character (bit 5)
        jnz 2f                          // If it's ready, jump forward to output
        testb %cl, %cl                  // It's not ready, test if we should block until it is
        jz 3f                           // We were told not to block, jump out
        XKSerialDelay %ax, $100         // Delay a little bit in between checks
        jmp 1b                          // Jump to check again

    2:
        movb %sil, %al                  // Move character to transmit into accumulator
        leaw (%edi), %dx                // Load the I/O port of transmit buffer (base) into I/O register (%dx)
        outb %al, %dx                   // Send the character to transmit into the transmit buffer
        movb $1, %al                    // Mark that we wrote the character
        ret                             // Get out of here (success)

    3:
        movb $0, %al                    // Mark that we didn't write anything
        ret                             // Get out of here (failure)

// Arguments:
//   %di: port
//   %sil: block
//
// Return Value (%al):
//   success: value read
//   nothing read: 0x00
//   read error: kXKSerialReadError
//
// Destroyed Registers:
//   %al
//   %dx
XKDeclareFunction(XKSerialReadCharacter):
    leaw 5(%edi), %dx                   // Load line status register (base + 5) into I/O register (%dx)

    1:
        inb %dx, %al                    // Load line status register into the accumulator
        testb $0x0E, %al                // Test to see if there was some kind of transmission error (bits 1-3)
        jnz 3f                          // Jump to return error if one occurred
        testb $1, %al                   // Test to see if a character has been received
        jnz 2f                          // A character has been received, jump to read it
        testb %sil, %sil                // Test to see if we should wait for a character
        jz 4f                           // If we shouldn't, just return 0
        XKSerialDelay %ax, $5           // Delay a little bit between checks (less than when writing)
        jmp 1b                          // Jump to check again

    2:
        leaw (%edi), %dx                // Load read buffer register (base) into I/O port register (%dx)
        inb %dx, %al                    // Load the waiting character into the accumulator
        ret                             // Return with the character we read

    3:
        movb $kXKSerialReadError, %al   // There was a read error (aww :/), signify this in the return value
        ret                             // Return with read error (which is the bell character)

    4:
        movb $0x0, %al                  // We didn't do anything, signify this
        ret                             // Return with nothing

// Arguments:
//   %di: port
//   %rsi: string pointer
//
// Return Value:
//   ---
//
// Destroyed Registers:
//   %al
//   %cl
//   %dx
//   %sil
//   %r10
XKDeclareFunction(XKSerialWriteString):
    movq %rsi, %r10                     // Save string pointer in r10
    movb $1, %cl                        // Set block parameter for write function to true

    1:
        movb (%r10), %sil               // Load next character from the string
        testb %sil, %sil                // Check to see if it's a null terminator
        jz 2f                           // If it is, don't print it just leave
        incq %r10                       // Increment the string pointer
        callq XKSymbol(XKSerialWriteCharacter)  // Print the character we just checked
        jmp 1b                          // Keep going until null terminator

    2:
        ret                             // Get out of here

#elif kCXArchARM && kCXBuildDev

#endif /* Arch & Dev */
