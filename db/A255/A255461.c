/*
 * A255461  A255460(2^n-1).
 * https://oeis.org/A255461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) - 7*a(n-3) + 6*a(n-5) - 8*a(n-7)
 *            (recurrence derived from the rational generating function)
 *            Initial values: a(0)=1, a(1)=6, a(2)=22, a(3)=82, a(4)=306,
 *            a(5)=1142, a(6)=4250
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, abort, free */
#ifdef __cplusplus
  #include <cstddef> /* for std::nullptr_t */
#endif

/* static cache for previously computed terms */
static seq_t *cache = NULL;
static size_t capacity = 0;        /* allocated size of cache   */
static size_t max_computed = (size_t)(-1);  /* largest index computed */

/* initialise the first 7 terms */
static void init_cache(void)
{
    capacity = 7;     /* indices 0..6 */
    #ifdef __cplusplus
        cache = new seq_t[capacity];
    #else
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
    #endif
    if (!cache) abort();
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)6;
    cache[2] = (seq_t)22;
    cache[3] = (seq_t)82;
    cache[4] = (seq_t)306;
    cache[5] = (seq_t)1142;
    cache[6] = (seq_t)4250;
    max_computed = 6;
}

/* expand the cache if needed, keeping existing values */
static void ensure_capacity(size_t n)
{
    if (!cache) {
        init_cache();
        if (n <= max_computed) return;
    }
    if (n < capacity) return;

    size_t new_cap = n + 1;
    seq_t *tmp = NULL;
    #ifdef __cplusplus
        tmp = new seq_t[new_cap];
        for (size_t i = 0; i < capacity; ++i) tmp[i] = cache[i];
        delete[] cache;
    #else
        tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { free(cache); abort(); }
        cache = tmp;
    #endif
    cache = tmp;
    capacity = new_cap;
}

/* compute all terms up to n (inclusive) */
static void compute_to(size_t n)
{
    if (!cache) init_cache();
    ensure_capacity(n);
    if (n <= max_computed) return;

    for (size_t i = max_computed + 1; i <= n; ++i) {
        /* a(i) = 5*a(i-1) - 3*a(i-2) - 7*a(i-3) + 6*a(i-5) - 8*a(i-7) */
        seq_t val = 5 * cache[i - 1];
        val -= 3 * cache[i - 2];
        val -= 7 * cache[i - 3];
        val += 6 * cache[i - 5];
        val -= 8 * cache[i - 7];
        cache[i] = val;
    }
    max_computed = n;
}

/* public entry point */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    compute_to(idx);
    return cache[idx];
}
