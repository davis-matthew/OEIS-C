/*
 * A241567  Number of 2-element subsets of {1,...,n} whose sum has more than 3
 *          divisors.
 * https://oeis.org/A241567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2‑element subsets {i,j} of {1..n} whose sum has
 *            strictly more than 3 divisors (i.e. τ(i+j) > 3)
 * Range:     u8 n <= 28, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;           /* a(1) = a(2) = a(3) = 0 */

    idx_t limit = 2 * n;             /* maximum possible sum */

    /* allocate divisor–count array up to 'limit' */
    size_t sz = (size_t)(limit + 1) * sizeof(idx_t);
    idx_t *tau = (idx_t *)malloc(sz);
    if (!tau) return 0;              /* allocation failure, unlikely */

    /* initialise to zero */
    for (idx_t i = 0; i <= limit; ++i) tau[i] = 0;

    /* sieve: count number of divisors for every integer <= limit */
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i) {
            ++tau[j];
        }
    }

    /* accumulate counts of admissible pairs */
    seq_t result = 0;
    for (idx_t s = 3; s <= limit; ++s) {
        if (tau[s] > 3) {
            idx_t count;
            if (s <= n + 1) {
                count = (s - 1) / 2;                 /* s <= n+1 case */
            } else {
                count = (2 * n - s + 1) / 2;           /* s > n+1 case */
            }
            result += (seq_t)count;
        }
    }

    free(tau);
    return result;
}
