/*
 * A397995  E.g.f. A(x) satisfies A(-log(1-4*x)/4) = 5*A(x) - 4*x.
 * https://oeis.org/A397995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n+1) = Σ_{k=1..n} 4^(n-k) * |S1(n+1,k)| * a(k) where
 *            |S1(n,k)| are unsigned Stirling numbers of the first kind. The
 *            function memoizes the sequence so that calls may come in any
 *            order
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* --- memoization tables --- */
    static seq_t *cache   = NULL;   /* a[1..cache_size]  */
    static idx_t  cache_size = 0;
    static idx_t  cache_cap  = 0;

    static seq_t *pow4   = NULL;   /* pow4[0..] = 4^i   */
    static idx_t  pow4_cap = 0;
    static idx_t  pow4_size = 0;

    /* Stirling numbers S1(n,k) for current n (row). 1‑based indexing. */
    static seq_t *s    = NULL;   /* s[1..s_size] */
    static idx_t  s_size = 0;
    static idx_t  s_cap  = 0;

    /* --- initialise if needed --- */
    if (cache_size == 0) {
        /* Allocate space for a[1] */
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* out of memory */
        cache[1] = (seq_t)1;
        cache_size = 1;

        /* pow4[0] = 1 */
        pow4_cap = 4;
        pow4 = (seq_t*)malloc(pow4_cap * sizeof(seq_t));
        if (!pow4) return 0;
        pow4[0] = (seq_t)1;
        pow4_size = 1;

        /* s for n = 1: S1(1,1) = 1 */
        s_cap = 4;
        s = (seq_t*)malloc(s_cap * sizeof(seq_t));
        if (!s) return 0;
        s[1] = (seq_t)1;
        s_size = 1;
    }

    /* --- ensure cache has up to n --- */
    while (cache_size < n) {
        /* current n is cache_size; we compute a[cache_size+1] */
        idx_t old_n = cache_size;          /* old_n >= 1 */
        idx_t new_n = old_n + 1;

        /* --- grow Stirling array if needed --- */
        if (s_size < new_n) {
            idx_t new_cap = s_cap * 2;
            if (new_cap < new_n) new_cap = new_n;
            seq_t *tmp = (seq_t*)realloc(s, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            s = tmp;
            s_cap = new_cap;
        }

        /* Update Stirling numbers for n = new_n in place (high → low). */
        /* s[new_n] = 1; for k = new_n-1 .. 1: s[k] = s[k-1] + old_n * s[k]; */
        s[new_n] = (seq_t)1;
        for (idx_t k = new_n; k >= 1; --k) {
            if (k == new_n) continue;
            /* s[k] = s[k-1] + old_n * s[k] */
            seq_t part = s[k];
            part = part * (seq_t)old_n;  /* multiply by old_n */
            s[k] = s[k-1] + part;
            if (k == 1) break; /* avoid underflow of unsigned k */
        }

        /* Ensure pow4 array covers needed exponents up to old_n - 1 */
        if (pow4_size <= old_n) {
            idx_t new_cap = pow4_cap * 2;
            if (new_cap < old_n + 1) new_cap = old_n + 1;
            seq_t *tmp = (seq_t*)realloc(pow4, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            pow4 = tmp;
            pow4_cap = new_cap;
        }
        /* compute pow4[old_n] if not present */
        if (pow4_size <= old_n) {
            pow4[old_n] = pow4[old_n - 1] * (seq_t)4;
            pow4_size = old_n + 1;
        }

        /* --- compute a[new_n] --- */
        seq_t sum = 0;
        for (idx_t k = 1; k <= old_n; ++k) {
            seq_t term = pow4[old_n - k];
            term = term * s[k];
            term = term * cache[k];
            sum = sum + term;
        }

        /* Grow cache if needed */
        if (cache_size + 1 >= cache_cap) {
            idx_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size + 1] = sum;
        cache_size = new_n;
    }

    return cache[n];
}
