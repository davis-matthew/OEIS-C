/*
 * A277130  Number of planar branching factorizations of n.
 * https://oeis.org/A277130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of planar branching factorizations of n defined
 *            recursively as a(1) = 0 a(n) = 1 + sum_{ordered factorization
 *            (f1,…,fk), k>=2} f(f1) * ... * f(fk) for n>1 The implementation
 *            evaluates this recurrence by enumerating all ordered
 *            multiplicative partitions of n
 * Range:     u8 n <= 47, u16 n <= 383, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   int_floor_sqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/int_floor_sqrt_u64.h"

/* Cached values of a(n).  The array is expanded as needed. */
static seq_t *a_cache   = NULL;   /* array of a(n) values */
static idx_t   cache_sz = 0;      /* allocated size of the array */
static idx_t   computed = 1;      /* highest n for which a_cache[n] is known */

/* Ensure the cache can hold values up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_sz > n) return;
    idx_t new_sz = n + 1;
    a_cache = (seq_t*)realloc(a_cache, new_sz * sizeof(seq_t));
    if (!a_cache) exit(EXIT_FAILURE);
    for (idx_t i = cache_sz; i < new_sz; ++i) a_cache[i] = 0;
    if (cache_sz == 0) a_cache[1] = 0;   /* a(1) = 0 */
    cache_sz = new_sz;
}

/* Forward declaration for recursive enumeration. */
static void dfs_factor(idx_t remain, seq_t prod, idx_t depth,
                       seq_t *sum, seq_t *values);

/* Compute a(n) for all n up to `n`. */
static void compute_up_to(idx_t n)
{
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        dfs_factor(i, (seq_t)1, (idx_t)0, &sum, a_cache);
        a_cache[i] = sum + (seq_t)1;  /* +1 for the leaf case */
    }
    computed = n;
}

/* Recursive enumeration of ordered multiplicative partitions of `remain`. */
static void dfs_factor(idx_t remain, seq_t prod, idx_t depth,
                       seq_t *sum, seq_t *values)
{
    /* Enumerate proper divisors of `remain` (d < remain). */
    idx_t limit = int_floor_sqrt_u64(remain);
    for (idx_t d = 2; d <= limit; ++d) {
        if (remain % d == 0) {
            idx_t d1 = d;
            idx_t d2 = remain / d;

            /* Branch with d1 as the next factor. */
            seq_t prod1 = prod * values[d1];
            dfs_factor(remain / d1, prod1, depth + 1, sum, values);

            /* Branch with the complementary divisor d2, if distinct. */
            if (d2 != d1) {
                seq_t prod2 = prod * values[d2];
                dfs_factor(remain / d2, prod2, depth + 1, sum, values);
            }
        }
    }

    /* Option: treat the whole remaining part as the next factor. */
    /* This contributes only when at least two factors are chosen. */
    if (depth + 1 >= 2) {
        seq_t prod_rem = prod * values[remain];
        *sum += prod_rem;
    }
}

/* Public function required by the interface. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    ensure_cache(n);
    if (computed < n) compute_up_to(n);
    return a_cache[n];
}
