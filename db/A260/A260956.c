/*
 * A260956  a(0)=1; a(n) = Sum_{k=1..n-1} d(k)*a(n-k), where d(m) is m-th bit
 *          in binary expansion of n.
 * https://oeis.org/A260956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=1, a(1)=1, a(2)=1; for n ≥ 3, a(n) = Σ_{k=1}^{n-1}
 *            d(k)·a(n‑k), where d(k) is the k‑th bit of n when the bits are
 *            numbered from the most significant one starting at 1 (bits
 *            beyond the binary length are considered 0)
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 50, u64 n <= 96, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- auxiliary functions ---------------------------------------- */

static inline unsigned floor_log2_uint64(idx_t x)
{
    /* works for x>0; for x==0 the caller ensures it is not used. */
#if defined(__GNUC__) || defined(__clang__)
    return 63 - __builtin_clzll((long long)x);
#else
    unsigned r = 0;
    while (x >>= 1) ++r;
    return r;
#endif
}

/* --- memoisation ----------------------------------------------- */

static seq_t *cache = NULL;        /* array of already computed a(n) */
static size_t  cap   = 0;          /* allocated capacity of cache      */
static size_t  cur   = 0;          /* number of computed entries (a[0]..a[cur-1]) */

/* Increase the cache size so that index 'needed-1' can be accessed. */
static void ensure_capacity(size_t needed)
{
    if (cap >= needed) return;
    size_t newcap = cap ? cap * 2 : 8;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();                     /* fatal if memory allocation fails */
    cache = tmp;
    cap   = newcap;
}

/* Compute the sequence values up to (and including) n. */
static void ensure_computed(idx_t n)
{
    if (n < (idx_t)cur) return;                /* already ready */
    ensure_capacity((size_t)n + 1);

    /* initialise base values if not yet initialised. */
    if (cur == 0)        { cache[0] = (seq_t)1; cur = 1; }
    if (cur <= 1)        { cache[1] = (seq_t)1; cur = 2; }
    if (cur <= 2)        { cache[2] = (seq_t)1; cur = 3; }

    for (idx_t i = cur; i <= n; ++i) {
        if (i <= 2) {                      /* cover 0,1,2 but they are already set */
            cache[i] = (seq_t)1;
            continue;
        }
        unsigned bitlen = 1 + floor_log2_uint64(i);   /* number of bits of i */
        seq_t sum = (seq_t)0;
        for (unsigned k = 1; k <= bitlen; ++k) {
            if ( ((i >> (bitlen - k)) & 1ULL) != 0ULL ) {
                /* bit k (from MSB) is 1, add a[i-k] */
                sum += cache[i - k];
            }
        }
        cache[i] = sum;
    }
    cur = (size_t)n + 1;
}

/* --------------------------------------------------------------------- */
/*  Public function matching the required signature                      */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    ensure_computed(n);
    return cache[n];
}
