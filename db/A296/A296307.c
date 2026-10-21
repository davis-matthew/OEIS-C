/*
 * A296307  Array read by upwards antidiagonals: f(n,k) =
 *          (n+1)*ceiling(n/(k-1)) - 1.
 * https://oeis.org/A296307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = f(n',k') where (n',k') are the coordinates of * the n‑th
 *            term when the array f(m,l) (m ≥ 1, l ≥ 2) is read by * upward
 *            antidiagonals; f(m,l) = (m+1)*ceil(m/(l-1)) – 1 * (OEIS A296307)
 * Range:     u8 n <= 120, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   triangular_term_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/triangular_term_count.h"

/*--- support functions ----------------------------------------------------*/

static idx_t find_sum_idx(idx_t m)
/* finds the smallest s ≥ 3 such that triangular_term_count(s) ≥ m  */
{
    idx_t low  = 3;
    idx_t high = 3;

    /* grow high until it is large enough */
    while (triangular_term_count(high) < m) {
        high <<= 1;           /* double until T(high)≥m */
    }

    /* binary search between low and high */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (triangular_term_count(mid) >= m)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;                /* this is the desired s */
}

/*--- sequence routine -----------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for offset 1, keep safe */
        return (seq_t)0;

    /* locate the antidiagonal (i.e. the constant sum s = n + k)  */
    idx_t s = find_sum_idx(n);

    /* number of terms before this antidiagonal */
    idx_t prev_count = triangular_term_count(s - 1);

    /* position inside this antidiagonal (1‑based) */
    idx_t p = n - prev_count;          /* 1 ≤ p ≤ s-2 */

    /* coordinates (n',k') inside the array */
    idx_t n_prime = s - p - 1;         /* n' = s - p - 1 (≥ 1) */
    idx_t k_prime = p + 1;             /* k' = p + 1 (≥ 2) */

    /* compute f(n',k') = (n'+1)*ceil(n'/(k'-1)) – 1 */
    seq_t n_val   = (seq_t)n_prime;
    seq_t k_val   = (seq_t)k_prime;

    seq_t ceil_val = (seq_t)((n_val + k_val - 2) / (k_val - 1));
    seq_t result   = (n_val + (seq_t)1) * ceil_val - (seq_t)1;

    return result;
}
