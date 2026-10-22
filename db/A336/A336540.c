/*
 * A336540  G.f. A(x) satisfies A(x) = 1 + x * A(x)^4 * (2 + A(x)).
 * https://oeis.org/A336540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{k=1..n} 3^k * binomial(n,k) *
 *            binomial(4*n,k-1) (including a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* if seq_t is 64 bit or smaller we use unsigned __int128
       for the temporary sum to avoid overflow of the intermediate
       result.  This is sufficient for all values that fit into
       seq_t (i.e. up to n = 14 for the 64 bit build).           */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sum = 0;
        unsigned __int128 power3 = 3;          /* 3^1 */
        idx_t four_n = 4 * n;

        for (idx_t k = 1; k <= n; ++k) {
            unsigned __int128 term =
                (unsigned __int128) binom(n, k) *
                (unsigned __int128) binom(four_n, k - 1);
            term *= power3;
            sum += term;

            power3 *= 3;
        }
        /* divide after the whole sum – the division is exact */
        return (seq_t)(sum / n);
    }
    /* arbitrary‑precision seq_t  – the calculations are exact   */
    seq_t sum = 0;
    seq_t power3 = 3;          /* 3^1 */
    idx_t four_n = 4 * n;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = binom(n, k);
        term *= binom(four_n, k - 1);
        term *= power3;
        sum += term;
        power3 *= 3;
    }
    /* division by n is exact for all n>0                       */
    return sum / n;
}
