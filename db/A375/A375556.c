/*
 * A375556  Expansion of e.g.f. 1 / (1 + x * log(1 - x^3/6)).
 * https://oeis.org/A375556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/3⌋} (n-3k)! * |Stirling1(k,n-3k)|
 *            /(6^k*k!)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence (used here):
   a(0)=1, a(1)=a(2)=a(3)=0,
   a(n) = Σ_{t=1}^{⌊(n-1)/3⌋} a(n-3t-1) * (n!/(n-3t-1)!)/(t·6^t) */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* External types supplied by the build system */
extern seq_t a(idx_t n);

/* Cache of previously computed terms */
static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;

/* Ensure a_cache holds values up to n */
static void ensure_a(idx_t n)
{
    if (n < a_cache_size)
        return;                     /* already computed */

    /* Extend cache to hold indices up to n */
    seq_t *tmp = (seq_t *)realloc(a_cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp)
        return;                     /* allocation failed – caller will handle */
    a_cache = tmp;

    /* Base case */
    if (a_cache_size == 0) {
        a_cache[0] = 1;
        a_cache_size = 1;
    }

    /* Compute new terms */
    for (idx_t i = a_cache_size; i <= n; ++i) {
        if (i <= 3) {                /* a(1)=a(2)=a(3)=0 */
            a_cache[i] = 0;
            continue;
        }

        seq_t sum = 0;
        idx_t t_max = (i - 1) / 3;
        for (idx_t t = 1; t <= t_max; ++t) {
            idx_t k   = i - 3 * t - 1;     /* previous index */
            idx_t m   = 3 * t + 1;         /* number of factors in the ratio */
            seq_t prod = a_cache[k];       /* start with a(k) */

            /* We will cancel factors of 2 and 3 as soon as possible */
            idx_t twos   = t;
            idx_t threes = t;
            /* Multiply by consecutive integers (k+1)..(k+m) */
            for (idx_t j = 1; j <= m; ++j) {
                prod *= (seq_t)(k + j);
                /* cancel factors 2 */
                while (twos > 0 && (prod & 1) == 0) {
                    prod >>= 1;
                    --twos;
                }
                /* cancel factors 3 */
                while (threes > 0 && prod % 3 == 0) {
                    prod /= 3;
                    --threes;
                }
            }
            /* finally divide by the remaining factor t */
            prod /= (seq_t)t;
            sum += prod;
        }
        a_cache[i] = sum;
    }
    a_cache_size = n + 1;
}

/* Public accessor */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    ensure_a(n);
    return a_cache[n];
}
