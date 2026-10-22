/*
 * A324175  Integers k such that floor(sqrt(k)) + floor(sqrt(k/2)) divides k.
 * https://oeis.org/A324175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A324175: Integers k such that floor(sqrt(k)) + floor(sqrt(k/2)) divides k. */
/*  This function builds the sequence lazily and caches the first n terms.  */

#include <stdlib.h>            /* for malloc/realloc */

static seq_t *cache = NULL;     /* cached qualifying k values */
static idx_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t cache_len = 0;     /* number of stored terms */
static idx_t next_k = 1;        /* next candidate integer to test */

/* ------------------------------------------------------------------ */
/* Integer square root for uint64_t: returns floor(sqrt(n)).           */
/* ------------------------------------------------------------------ */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t lo = 1;
    idx_t hi = (n >> 1) + 1;        /* sqrt(n) <= n/2 + 1 for n >= 4 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;   /* upper mid */
        if (mid <= n / mid) lo = mid;
        else                hi = mid - 1;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/* Grow the cache array when needed.                                  */
/* ------------------------------------------------------------------ */
static void ensure_cache_cap(void)
{
    if (cache_len < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) exit(1);  /* unrecoverable memory failure */
    cache = p;
    cache_cap = (idx_t)new_cap;
}

/* ------------------------------------------------------------------ */
/* Main entry point.                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n==0, return 0 */

    /* Generate and cache terms until we have the nth term. */
    while (cache_len < n) {
        /* Test candidate next_k until we find a valid k. */
        while (1) {
            idx_t a1 = isqrt_u64(next_k);
            idx_t a2 = isqrt_u64(next_k / 2);
            idx_t denom = a1 + a2;

            if (denom && (next_k % denom == 0)) {
                /* Valid term found; store it. */
                ensure_cache_cap();
                cache[cache_len] = (seq_t)next_k;
                cache_len++;
                break;
            }
            next_k++;  /* try the next integer */
        }
        next_k++;      /* skip the last found k for next search */
    }

    return cache[n - 1];
}
