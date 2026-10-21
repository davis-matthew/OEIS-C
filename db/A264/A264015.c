/*
 * A264015  Number of (n+1) X (6+1) arrays of permutations of 0..n*7+6 with
 *          each element having index change +-(.,.) 0,0 1,2 or 2,2.
 * https://oeis.org/A264015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 60*a(n-1) - 300*a(n-2) + 1500*a(n-3) - 7500*a(n-4) +
 *            3125*a(n-5) for n > 7, with initial values a(1)=32, a(2)=1869,
 *            a(3)=146410, a(4)=7320500, a(5)=398967250, a(6)=21860843125,
 *            a(7)=1201888840625
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache for computed values. */
static seq_t *cache = NULL; /* elements stored as a(1) .. a(n) */
static size_t cache_cap = 0;/* allocated capacity of cache   */
static size_t cache_len = 0;/* number of values currently stored */

/* Initial values for n = 1 .. 7. */
static const seq_t base[7] = {
    (seq_t)32ULL,
    (seq_t)1869ULL,
    (seq_t)146410ULL,
    (seq_t)7320500ULL,
    (seq_t)398967250ULL,
    (seq_t)21860843125ULL,
    (seq_t)1201888840625ULL
};

/* Ensure 'cache' has room for at least 'n' elements. */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n;
    if (need <= cache_cap)
        return;                        /* already enough space */

    size_t new_cap = need;              /* simple linear expansion */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        abort();                       /* allocation failure */
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for n < 1; return 0 */
        return (seq_t)0;

    /* If value already cached, return it. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Make sure the cache can hold all values up to 'n'. */
    ensure_cache(n);

    /* Compute all missing terms up to 'n'. */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        if (i < 7) {                /* base values */
            cache[i] = base[i];
        } else {                    /* recurrence for i >= 7 (i = n-1) */
            seq_t a1 = cache[i - 1]; /* a(i)  = a(n-1)   */
            seq_t a2 = cache[i - 2]; /* a(i-1)= a(n-2)   */
            seq_t a3 = cache[i - 3]; /* a(i-2)= a(n-3)   */
            seq_t a4 = cache[i - 4]; /* a(i-3)= a(n-4)   */
            seq_t a5 = cache[i - 5]; /* a(i-4)= a(n-5)   */

            /* Compute 60*a(n-1) + 1500*a(n-3) + 3125*a(n-5)  */
            seq_t pos = (seq_t)60 * a1 + (seq_t)1500 * a3 + (seq_t)3125 * a5;
            /* Compute 300*a(n-2) + 7500*a(n-4)              */
            seq_t neg = (seq_t)300 * a2 + (seq_t)7500 * a4;

            cache[i] = pos - neg;   /* guaranteed non‑negative */
        }
    }

    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
