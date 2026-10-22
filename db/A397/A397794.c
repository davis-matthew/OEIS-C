/*
 * A397794  a(n) is the decimal encoding of the unique snowball numeral
 *          <d_k...d_1> having value n.
 * https://oeis.org/A397794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithmic computation of OEIS A397794:
   decimal encoding of the snowball numeral <d_k...d_1> for n.
   Uses mixed-radix conversion: base_1 = 2, base_i = d_{i-1}+2. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Representation of <0> */

    seq_t val = (seq_t)n;          /* Working value (n in mixed radix) */
    seq_t result  = 0;             /* Decimal encoding being built */
    seq_t pow10   = 1;             /* 10^position for the current digit */
    seq_t base    = 2;             /* Current radix (starts at 2) */

    while (val > 0) {
        seq_t digit = val % base;          /* Least‑significant digit */
        result += digit * pow10;           /* Append digit to decimal string */
        val      /= base;                  /* Remove processed digit */
        base     = digit + 2;              /* Next radix depends on digit */
        pow10   *= 10;                     /* Shift decimal position */
    }
    return result;
}
