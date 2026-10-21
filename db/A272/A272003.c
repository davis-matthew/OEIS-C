/*
 * A272003  Decimal expansion of the molar specific heat of an ideal gas
 *          consisting of molecules with 5 degrees of freedom at constant
 *          pressure, in J mol^-1 K^-1.
 * https://oeis.org/A272003
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  cons, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A272003: decimal digits of 7/2 R (where R is the gas constant
   with 14 decimal places: 8.3144598...).  The integer
   2910061916353634 contains the digits of 29.10061916353634.
   The sequence is a(2)=2, a(3)=9, … a(17)=4, i.e. a(n) is the
   (n‑2)nd digit of that integer in base‑10.  For n > 17 all
   subsequent terms are 0. */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2)
        return 0; /* undefined below offset */

    static const char digits[] = "2910061916353634";
    idx_t pos = n - 2;
    if (pos < sizeof(digits) - 1) /* exclude terminating NUL */
        return (seq_t)(digits[pos] - '0');
    return 0;
}
