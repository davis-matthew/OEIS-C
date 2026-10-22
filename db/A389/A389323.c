/*
 * A389323  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+4*k-1,n-k).
 * https://oeis.org/A389323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+4*k-1,n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    seq_t total = 0;
    seq_t binom_nk = 1;   // C(n,0)
    for (idx_t k = 0; k <= n; ++k) {
        idx_t bigN = n + 4 * k - 1;
        idx_t bigK = n - k;
        seq_t term2 = binomial(bigN, bigK);
        total += binom_nk * term2;

        if (k == n) break;

        /* Update binom_nk to C(n,k+1) using the recurrence:
           C(n,k+1) = C(n,k) * (n - k) / (k + 1)   (exact division) */
        binom_nk = (binom_nk * (seq_t)(n - k)) / (seq_t)(k + 1);
    }
    return total;
}
