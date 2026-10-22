/*
 * A332533  a(n) = (1/n) * Sum_{k=1..n} floor(n/k) * n^k.
 * https://oeis.org/A332533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d=1..n} floor(n/d) * n^d
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                      /* n as seq_t */
    /* Fast path for small seq_t (<= 8 bytes).  Use 128‑bit temporaries
       to avoid overflow while the final result still fits in seq_t. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sum = 0;
        unsigned __int128 pow = (unsigned __int128)sn;   /* n^1 */
        const unsigned __int128 S = (unsigned __int128)n;/* n as 128‑bit */

        for (idx_t d = 1; d <= n; ++d) {
            unsigned __int128 factor = S / d;           /* floor(n/d) */
            sum += pow * factor;                       /* add term */
            if (d != n)
                pow *= S;                              /* next power n^(d+1) */
        }
        return (seq_t)(sum / S);                        /* divide by n */
    }

    /* General case (arbitrary‑precision seq_t) */
    seq_t sum = 0;
    seq_t pow = sn;                                      /* n^1 */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t factor = sn / (seq_t)d;                     /* floor(n/d) */
        sum += factor * pow;                             /* add term */
        if (d != n)
            pow *= sn;                                   /* next power */
    }
    return sum / sn;                                      /* divide by n */
}
