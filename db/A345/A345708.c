/*
 * A345708  a(n) is the least positive number starting an interval of
 *          consecutive integers whose product of elements is n.
 * https://oeis.org/A345708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least starting number m such that * m · (m+1) · … ·
 *            (m+k−1) = n for some k ≥ 1. * The algorithm tests all possible
 *            lengths k (k ≥ 2) for which k! ≤ n * and scans m from 1 upward.
 *            As soon as a product equals n, * the current m is the minimal
 *            start for that k, and if it is smaller * tha...
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;          /* convert index to seq_t for arithmetic */
    idx_t best_m = n;               /* worst case: k=1 gives m=n */

    /* Iterate over k ≥ 2 with factorial(k) ≤ n */
    seq_t fac = (seq_t)1;           /* factorial(k-1) */
    for (idx_t k = 2; ; ++k) {
        /* Update factorial: fac *= k */
        if (fac > nseq / (seq_t)k)     /* would exceed nseq – stop */
            break;
        fac *= (seq_t)k;
        if (fac > nseq)                /* k! > n – stop searching lengths */
            break;

        /* Search starting point m */
        for (idx_t m = 1; m <= n; ++m) {
            if (m >= best_m)           /* cannot beat current best */
                break;
            seq_t prod = (seq_t)1;
            bool ok = true;
            for (idx_t i = 0; i < k; ++i) {
                seq_t term = (seq_t)(m + i);
                if (prod > nseq / term) {  /* would overflow / exceed n */
                    ok = false;
                    break;
                }
                prod *= term;
            }
            if (ok && prod == nseq) {
                best_m = m;            /* found a better start for this k */
                break;                 /* m increases, next will be larger */
            }
        }

        if (best_m == 1)                /* cannot find a start smaller than 1 */
            break;
    }
    return (seq_t)best_m;
}
