/*
 * A260317  Numbers not of the form v(m) + v(n), where v = A001950 (upper
 *          Wythoff numbers) and 1 <= m <= n - 1, for n >= 2.
 * https://oeis.org/A260317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth positive integer NOT equal to v(m) + v(n) where 1
 *            ≤ m < n and v(k) = floor(k*φ) + k (upper Wythoff numbers, φ =
 *            (1+√5)/2)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the set of all such sums up to a bound
   5*n + 10 and returns the first n elements that are not present in
   that set. It works for any unsigned seq_t type and for the
   arbitrary‑precision variant. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Long‑double approximation of the golden ratio.  It is accurate
   enough for k up to several millions, which covers all values
   needed for the tests. */
static const long double PHI = 1.6180339887498948482045868343656L;

/* Static cache of previously computed a(1)..a(nmax). */
static seq_t *seq_cache = NULL;     /* cache of a(1)..a(nmax)  */
static size_t cache_cap = 0;        /* number of values stored   */
static idx_t  cache_nmax = 0;       /* maximum n known           */

/* ------------------------------------------------------------------ */
/* Build the sequence up to 'up_to_n' terms.  All calculations are
   performed with idx_t so that the implementation works even when
   seq_t is small (e.g. uint8_t).  Only the final values are cast
   into seq_t. */
static void build_sequence(size_t up_to_n)
{
    /* Guarantee a(n) < 5n, so the 59th term is < 295.  Add a safety
       margin of ten. */
    size_t limit = 5 * up_to_n + 10;   /* inclusive upper bound */

    /* ----------------------------------------------------------------
       Generate all upper Wythoff numbers v(k) ≤ limit.
       We store them in an idx_t array to avoid overflow when seq_t
       is small. */
    /* ---------------------------------------------------------------- */
    size_t wcap   = 128;
    idx_t *wv     = (idx_t *)malloc(wcap * sizeof(idx_t));
    if (!wv) { free(seq_cache); exit(EXIT_FAILURE); }
    size_t wlen   = 0;

    for (size_t k = 1; ; ++k) {
        long double t = (long double)k * PHI;   /* k*φ    */
        idx_t floor_kphi = (idx_t)t;            /* floor(kφ) */
        idx_t v = floor_kphi + k;               /* v(k)    */
        if (v > limit) break;

        if (wlen >= wcap) {
            wcap <<= 1;
            idx_t *tmp = (idx_t *)realloc(wv, wcap * sizeof(idx_t));
            if (!tmp) { free(wv); free(seq_cache); exit(EXIT_FAILURE); }
            wv = tmp;
        }
        wv[wlen++] = v;
    }

    /* ----------------------------------------------------------------
       Mark all numbers that can be written as v(i)+v(j) with i<j.
       The `used` array is indexed by integers up to `limit`. */
    /* ---------------------------------------------------------------- */
    bool *used = (bool *)calloc(limit + 1, sizeof(bool));
    if (!used) { free(wv); free(seq_cache); exit(EXIT_FAILURE); }

    for (size_t i = 0; i < wlen; ++i) {
        idx_t vi = wv[i];
        for (size_t j = i + 1; j < wlen; ++j) {
            idx_t sum = vi + wv[j];
            if (sum > limit) break;            /* further j give larger sums */
            used[sum] = true;
        }
    }

    /* ----------------------------------------------------------------
       Build the new cache with the first `up_to_n` numbers not marked
       in `used`.  These values are cast to seq_t before storing. */
    /* ---------------------------------------------------------------- */
    seq_t *new_seq = (seq_t *)malloc(up_to_n * sizeof(seq_t));
    if (!new_seq) { free(used); free(wv); free(seq_cache); exit(EXIT_FAILURE); }

    size_t pos = 0;
    for (size_t x = 1; x <= limit && pos < up_to_n; ++x) {
        if (!used[x]) new_seq[pos++] = (seq_t)x;
    }

    /* ---------------------------------------------------------------
       Replace the old cache.  The old cache is freed and the new one
       becomes the current cache.  Since the caller will never refer to
       the old cache after this point, no further memory safety issue
       arises. */
    /* --------------------------------------------------------------- */
    free(seq_cache);
    seq_cache = new_seq;
    cache_cap = up_to_n;
    cache_nmax = (idx_t)up_to_n;

    /* Clean up temporary arrays. */
    free(used);
    free(wv);
}

/* ------------------------------------------------------------------- */
/* Public interface required by the build system.  It can be called with
   any n ≥ 1, in any order.  The cache is extended only when required
   and remains valid for all subsequent calls. */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 0 */

    /* Extend the cache if needed. */
    if ((size_t)n > cache_nmax) {
        build_sequence((size_t)n);
    }

    /* a(1) is stored at seq_cache[0]. */
    return seq_cache[(size_t)n - 1];
}
