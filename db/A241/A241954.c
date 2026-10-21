/*
 * A241954  Number of integers x such that the repeated application of
 *          sigma(x)->x leads to n.
 * https://oeis.org/A241954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers x such that repeated application of *
 *            sigma(x) eventually leads to n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static state to memoize up to the largest n requested */
    static idx_t max_prepared = 0;
    static uint32_t *sigma = NULL;  /* sigma values up to max_prepared */
    static seq_t *cnt   = NULL;     /* a(n) values up to max_prepared */

    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    if (n > max_prepared) {
        size_t sz = (size_t)n + 1;               /* array size 0..n */

        /* grow or allocate arrays */
        sigma = (uint32_t *)realloc(sigma, sz * sizeof(uint32_t));
        cnt   = (seq_t *)realloc(cnt,   sz * sizeof(seq_t));

        /* initialise to zero */
        memset(sigma, 0, sz * sizeof(uint32_t));
        memset(cnt,   0, sz * sizeof(seq_t));

        /* divisor sum σ(m) for all m <= n */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i) {
                sigma[j] += (uint32_t)i;
            }
        }

        /* dynamic programming over the inverse σ graph */
        cnt[1] = (seq_t)1;                     /* σ(1) = 1 */
        for (idx_t p = 2; p <= n; ++p) {
            uint32_t m = sigma[p];
            if (m <= n) {
                cnt[m] += cnt[p] + (seq_t)1;   /* p itself + its ancestors */
            }
        }

        max_prepared = n;
    }

    return cnt[n];
}
