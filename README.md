# Emulating the Intel 8080
http://emulator101.com/

## Build

```bash
make
```

Or with another C compiler:

```bash
CC=clang make
```

With debug symbols:

```bash
make clean && make debug
```

## Run

```bash
./intel8080 invaders/invaders
```

## Notes

### Parity

Need to check what the Parity flag is actually for. It seems that [this reference JavaScript implementation](https://bluishcoder.co.nz/js8080/) sets the flag based on the _value_ of the result rather than the number of bits set.

At line 287 of `js8080.js`:

```js
if (x % 2)
  this.f &= ~PARITY & 0xFF;
else
  this.f |= PARITY;
```

From the manual:

> If the modulo 2 sum of the bits of the result of the operation is 0, (ie., if the result has even parity), this flag is set; otherwise it is reset (ie., if the result has odd parity).

### Interrupts

Another difference found is at instruction 42434, the js emulator processes an interrupt. I have not yet emulated this.

### GDB

Can't get gdb to work on my system for some reason. Just hangs on new thread even though I set breakpoints.

## References
* [8080 opcodes](http://www.emulator101.com/reference/8080-by-opcode.html)
* [8080 assembly programming manual](http://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf)

