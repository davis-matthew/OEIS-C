/*
 * A260708  a(2n) = n*(2*n+1), a(2n+7) = a(2n+1) + 12*n + 28, with a(1)=1,
 *          a(3)=6, a(5)=16.
 * https://oeis.org/A260708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = n*(2*n+1); a(6k+1)=18k^2+10k+1; a(6k+3)=18k^2+22k+6;
 *            a(6k+5)=18k^2+34k+16; a(0)=0. Definitions for all n ≥ 0
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if ((n & 1) == 0)  /* even n: n = 2k  →  a(n) = k*(n+1) */
    {
        return ((seq_t)(n / 2)) * ((seq_t)n + 1);
    }
    else                /* odd n */
    {
        const idx_t r = n % 6;      /* 1,3 or 5 */
        const idx_t k = (n - r) / 6;
        const seq_t K = (seq_t)k;
        switch (r)
        {
            case 1:  /* 6k+1 */
                return (seq_t)18 * K * K + (seq_t)10 * K + 1;
            case 3:  /* 6k+3 */
                return (seq_t)18 * K * K + (seq_t)22 * K + 6;
            case 5:  /* 6k+5 */
                return (seq_t)18 * K * K + (seq_t)34 * K + 16;
            default: /* unreachable */
                return 0;
        }
    }
}
