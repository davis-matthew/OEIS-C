/*
 * A256626  Expansion of psi(x) / psi(x^3) in powers of x where psi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A256626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 if n is a triangular number, otherwise 0, * minus the
 *            sum of a(n - 3*T(m)) for all m ≥ 1 with * 3*T(m) ≤ n, where T(m)
 *            = m(m+1)/2. * This satisfies the convolution ∑_{k≥0} a(k) ·
 *            x^{3*T(k)} = ψ(x), * i.e. the expansion of ψ(x)/ψ(x³) in powers
 *            of x
 * Range:     i8 n <= 77, i16 n <= 219, i32 n <= 686, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- state for memoized values ---------- */
static seq_t *cache = NULL;          /* array containing a(0)…a(max)      */
static size_t cache_len = 0;         /* allocated length of cache          */
static idx_t   cache_max = 0;        /* largest index that has been filled */

/* ---------- helper ---------- */
static bool is_triangular(idx_t n)
{
    for (idx_t j = 0; ; ++j) {
        unsigned __int128 t = (unsigned __int128)j * (j + 1) / 2;
        if (t > (unsigned __int128)n) break;
        if (t == (unsigned __int128)n) return true;
    }
    return false;
}

/* Extend the cache so that a(n) is available and return nothing */
static void ensure_cache(idx_t n)
{
    if (n < cache_max) return;          /* already available */

    /* grow the allocation if necessary */
    if ((size_t)(n + 1) > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_len = new_len;
    }

    /* compute new terms */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t c = is_triangular(i) ? 1 : 0;   /* coefficient from ψ(x)  */
        seq_t sum = 0;
        for (idx_t m = 1; ; ++m) {
            unsigned __int128 t3 = (unsigned __int128)3 * m * (m + 1) / 2;
            if (t3 > (unsigned __int128)i) break;
            sum += cache[i - (idx_t)t3];
        }
        cache[i] = c - sum;
    }

    cache_max = n;
}

/* ---------- public entry point ---------- */
seq_t a(idx_t n)
{
    /* first call initialisation */
    static bool init_done = false;
    if (!init_done) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;          /* a(0) = 1 */
        cache_max = 0;
        init_done = true;
    }

    ensure_cache(n);
    return cache[n];
}
