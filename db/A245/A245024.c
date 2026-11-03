/*
 * A245024  Even numbers n for which lpf(n-1) < lpf(n-3), where lpf = least
 *          prime factor.
 * https://oeis.org/A245024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = even even integer n such that lpf(n-1) < lpf(n-3) (OEIS
 *            A245024). The sequence is memoised; successive calls cause
 *            on‑demand generation of further terms
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9943 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;         /* stored terms */
static size_t cache_cap = 0;        /* allocated capacity of cache    */
static idx_t  cache_len = 0;        /* number of terms already cached */
static idx_t  next_even = 10;       /* smallest even number not yet tested */

/* Return the nth term (1‑based index).  Behaviour for n < 1 is undefined. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safety check – not used */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        idx_t x   = next_even;             /* candidate even integer   */
        idx_t lpf1 = lpf(x - 1);           /* least prime factor of x-1 */
        idx_t lpf2 = lpf(x - 3);           /* least prime factor of x-3 */

        if (lpf1 < lpf2) {                /* qualifies for the sequence? */
            /* enlarge cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();         /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)x; /* store the term          */
        }
        next_even += 2;                    /* move to next even integer */
    }
    return cache[n - 1];                   /* terms are 1‑indexed */
}
