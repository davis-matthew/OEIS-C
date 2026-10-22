/*
 * A307241  a(0) = 1; a(n) = Sum_{k=1..n} (-1)^(k+1)*d(k+1)*a(n-k), where d()
 *          is the number of divisors (A000005).
 * https://oeis.org/A307241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A307241
 *
 *  a(0) = 1
 *  a(n) = Sum_{k=1..n} (-1)^(k+1) * d(k+1) * a(n-k)
 *
 *  where d(m) is the number of positive divisors of m.
 *
 *  Initial terms: a(0)=1, a(1)=2, a(2)=2, a(3)=3, a(4)=6, a(5)=12
 */
#include <stdlib.h>

static seq_t *a_cache = NULL;       /* cached a(0..cache_size-1)            */
static seq_t *d_cache = NULL;       /* cached divisor counts d(0..d_cache-1) */
static idx_t cache_size   = 0;      /* allocated size for a_cache           */
static idx_t dcache_size  = 0;      /* allocated size for d_cache           */
static idx_t computed_n   = 0;      /* highest n for which a_cache[n] computed */

/* allocation helper – works for both C and C++ */
static seq_t* alloc_array(idx_t len)
{
#ifdef __cplusplus
    return new seq_t[len];
#else
    return (seq_t*)malloc(sizeof(seq_t) * (size_t)len);
#endif
}

/* deallocation helper – works for both C and C++ */
static void free_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* ensure that a_cache[0..target_n] and d_cache[0..target_n+1] are valid   */
static void extend_cache(idx_t target_n)
{
    if (target_n <= computed_n) return;          /* nothing to do */

    /* new capacities */
    idx_t new_a_cap  = target_n + 1;              /* indices 0..target_n   */
    idx_t new_d_cap  = target_n + 2;              /* indices 0..target_n+1 */

    /* allocate new arrays */
    seq_t *new_a = alloc_array(new_a_cap);
    seq_t *new_d = alloc_array(new_d_cap);

    /* copy existing data */
    if (a_cache) {
        for (idx_t i = 0; i <= computed_n && i < cache_size; ++i)
            new_a[i] = a_cache[i];
        free_array(a_cache);
    }
    a_cache     = new_a;
    cache_size  = new_a_cap;

    if (d_cache) {
        size_t copy_len = (size_t)(computed_n + 1 < dcache_size ? computed_n + 1 : dcache_size);
        for (idx_t i = 0; i < copy_len; ++i)
            new_d[i] = d_cache[i];
        free_array(d_cache);
    }
    d_cache     = new_d;
    dcache_size = new_d_cap;

    /* initialise divisor array */
    for (idx_t i = 0; i < new_d_cap; ++i)
        new_d[i] = (seq_t)0;

    /* compute all divisor counts d(1)..d(target_n+1) */
    for (idx_t i = 1; i <= target_n + 1; ++i) {
        for (idx_t j = i; j <= target_n + 1; j += i)
            new_d[j] += (seq_t)1;
    }

    /* base value a(0) = 1 */
    if (computed_n < 1)
        new_a[0] = (seq_t)1;

    /* recurrence to fill a(target_n) onward */
    for (idx_t n = computed_n + 1; n <= target_n; ++n) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term = new_d[k + 1] * new_a[n - k];
            if (k & 1u)       /* odd k → plus sign */
                sum += term;
            else              /* even k → minus sign */
                sum -= term;
        }
        new_a[n] = sum;
    }

    computed_n = target_n;
}

/* public sequence routine */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    extend_cache(n);
    return a_cache[n];
}
