/*
 * A336877  The harmonic function on the Sierpinski gasket with vertices 0, 1,
 *          w = (-1)^(1/3) defined by the values a(0) = 0, a(1) = 1, a(w) =
 *          -1.
 * https://oeis.org/A336877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2*n)=5*a(n), a(m+k)=2*a(m)-a(m-k)+3*a(k) where
 *            m=2^p is the largest power of two ≤ n (0<k<m)
 * Range:     u8 n <= 11, u16 n <= 121, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* computed values */
static unsigned char *done = NULL;   /* 0 = not computed, 1 = computed */
static idx_t cache_cap = 0;          /* current size of the arrays */

/* enlarge the cache so that it can store indices up to 'n' */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    idx_t new_cap = 1;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    unsigned char *new_done = (unsigned char *)realloc(done, new_cap);
    if (!new_cache || !new_done) {
        /* allocation failure – terminate */
        abort();
    }
    /* initialise the newly allocated slots */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_done[i] = 0;
    cache = new_cache;
    done = new_done;
    cache_cap = new_cap;
}

/* largest power of two not exceeding n (n>0) */
static idx_t highest_pow2(idx_t n)
{
    idx_t p = 1;
    while (p <= (n >> 1))
        p <<= 1;
    return p;
}

/* recursive computation with memoisation */
static seq_t a_rec(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    ensure_cache(n);
    if (done[n])
        return cache[n];

    seq_t result;
    if ((n & 1) == 0) { /* even: a(2*n) = 5*a(n) */
        result = (seq_t)5 * a_rec(n >> 1);
    } else { /* odd: n = m + k with m = 2^p, 0<k<m */
        idx_t m = highest_pow2(n);
        idx_t k = n - m;
        seq_t a_m = a_rec(m);
        seq_t a_k = a_rec(k);
        seq_t a_m_minus_k = a_rec(m - k);
        result = (seq_t)2 * a_m - a_m_minus_k + (seq_t)3 * a_k;
    }

    cache[n] = result;
    done[n] = 1;
    return result;
}

seq_t a(idx_t n)
{
    return a_rec(n);
}
