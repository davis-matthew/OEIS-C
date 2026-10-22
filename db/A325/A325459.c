/*
 * A325459  Sum of numbers of nontrivial divisors (greater than 1 and less
 *          than k) of k for k = 1..n.
 * https://oeis.org/A325459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006218(n) - 2*n + 1, where A006218(n) is the sum of the
 *            divisor counts d(k) for k = 1..n. A006218 counts all divisors,
 *            so a(n) effectively sums the non‑trivial divisors of each k
 * Range:     u8 n <= 93, u16 n <= 9023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* static cache for divisor counts, expanded as needed */
    static unsigned int *div_cnt = NULL;
    static idx_t cap = 0;

    if (cap <= n) {
        free(div_cnt);
        cap = n + 1;
        /* cast malloc result for C++ compatibility */
        div_cnt = (unsigned int *)malloc(cap * sizeof(unsigned int));
        if (!div_cnt)                 /* out‑of‑memory – give up */
            return (seq_t)0;
    }

    /* clear divisor counts for indices up to n */
    for (idx_t i = 0; i <= n; ++i)
        div_cnt[i] = 0;

    /* compute divisor counts by a simple sieve */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i)
            ++div_cnt[j];
    }

    /* accumulate the sum of divisor counts
       (use seq_t for the accumulator to avoid overflow on big builds) */
    seq_t total = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i)
        total += (seq_t)div_cnt[i];

    /* apply the formula: total - 2*n + 1 */
    seq_t result = total - (seq_t)(2 * n) + (seq_t)1;
    return result;
}
