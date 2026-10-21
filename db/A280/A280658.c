/*
 * A280658  Numbers ending with their digital root in decimal representation.
 * https://oeis.org/A280658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10*n-19-9*((n-2) mod 9) for n>1, a(1)=0
 * Range:     u8 n <= 28, u16 n <= 6562, u32 n <= 9982, u64 n <= 9982,
 *            u128 n <= 9982, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;                     /* a(1) = 0 */

    /* (n-2) mod 9, computed with idx_t then cast to seq_t */
    idx_t mod = (n > 1) ? (n - 2) % 9 : 0;
    seq_t result = (seq_t)n * (seq_t)10;  /* 10*n */
    result -= (seq_t)19;                  /* -19 */
    result -= (seq_t)9 * (seq_t)mod;      /* -9*((n-2) mod 9) */
    return result;
}
