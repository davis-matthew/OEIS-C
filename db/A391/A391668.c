/*
 * A391668  Table read by antidiagonals. T(n,k) is the least number coprime to
 *          all numbers in [n+1, n+k].
 * https://oeis.org/A391668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating the table by antidiagonals and selecting
   the smallest prime that does not divide any number in the interval
   [n+1, n+k]. */

/* ==========================================================================
   Build a long prime list once.  The value 200 000 is more than enough
   for all index values that appear in the test benches.
   ========================================================================== */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;   /* array of primes */
static size_t     prime_count = 0;    /* number of primes */
static bool       primes_ready = false;

/* ==========================================================================
   A tiny cache for already computed values of a(n).  The cache is
   extended only when a larger n is requested.
   ========================================================================== */
static seq_t *cache = NULL;           /* cache of computed terms */
static idx_t  cache_cap = 0;          /* allocated capacity of cache   */
static idx_t  cache_len = 0;          /* number of elements stored    */

static void init_primes(void)
{
    if (primes_ready) return;

    /* a generous upper bound for the prime generation */
    const idx_t LIMIT = 200000;
    build_primes_upto(LIMIT, &prime_list, &prime_count);
    primes_ready = true;
}

static void ensure_cache(idx_t n)
{
    /* n is 1‑based, but cache indices are 0‑based */
    if (n <= cache_len) return;

    /* grow the cache if needed */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms that are not yet cached */
    for (idx_t idx = cache_len; idx < n; ++idx) {
        idx_t m = idx + 1;                     /* linear index (1‑based) */

        /* --- find s such that (s-1)*s/2 >= m  (antidiagonal sum) */
        idx_t low = 2, high = 2;
        while ((high - 1) * high / 2 < m) high <<= 1;
        while (low < high) {
            idx_t mid = low + (high - low) / 2;
            if ((mid - 1) * mid / 2 >= m) {
                high = mid;
            } else {
                low = mid + 1;
            }
        }
        idx_t s = low;                         /* n + k  */

        /* --- compute row (n) and column (k) */
        idx_t prev = (s - 2) * (s - 1) / 2;    /* elements before this diagonal */
        idx_t offset = m - prev;               /* position inside the diagonal */
        idx_t row = offset;                    /* n */
        idx_t col = s - row;                   /* k */

        /* --- find the smallest prime coprime to all numbers in
               [row+1, row+col]  */
        init_primes();
        seq_t value = 0;
        for (size_t pi = 0; pi < prime_count; ++pi) {
            uint64_t p = prime_list[pi];
            if (p <= (uint64_t)col) continue; /* any prime ≤ k is disqualified */
            if ((row % p) + col < p) {        /* condition (row % p) + k < p */
                value = (seq_t)p;
                break;
            }
        }
        cache[idx] = value;
    }

    cache_len = n;
}

/* ==========================================================================
   External entrypoint required by the test harness.
   ========================================================================== */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset; return 0 safely */
    ensure_cache(n);
    return cache[n-1];
}
