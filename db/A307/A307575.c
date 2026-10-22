/*
 * A307575  Number of Motzkin meanders of length n with an even number of
 *          peaks.
 * https://oeis.org/A307575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 16, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   coeff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A307575: Number of Motzkin meanders of length n with an even number of peaks.
   Recurrence (valid for n ≥ 8):
      -3*(n+1)*(n-2)*a(n)
      +4*(4*n^2-7*n-3)*a(n-1)
      +3*(-7*n^2+17*n-2)*a(n-2)
      +4*n*(n-3)*a(n-3)
      -(n-3)*(25*n-82)*a(n-4)
      +4*(n-3)*(6*n-19)*a(n-5)
      +(61*n^2-575*n+1302)*a(n-6)
      -4*(11*n-37)*(n-6)*a(n-7)
      -12*(n-6)*(n-7)*a(n-8) = 0
   Initial values (n = 0 … 7):
      a(0)=1, a(1)=2, a(2)=4, a(3)=9, a(4)=22,
      a(5)=56, a(6)=148, a(7)=402
   The function memoises results for all n ≥ 0 and works for
   arbitrary‑precision seq_t as well as for fixed‑size unsigned types.
*/

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc, free */
#include "../utils/coeff.h"

/* Initial terms (metadata only: 0‑based) */
static const uint64_t base_terms_full[8] = {
    1ULL, 2ULL, 4ULL, 9ULL, 22ULL, 56ULL, 148ULL, 402ULL
};

/* Cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Allocate or grow cache to accommodate at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise cache with the first eight terms. */
static void init_cache(void)
{
    if (cache) return;
    cache_cap = 8;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();
    for (size_t i = 0; i < 8; ++i) cache[i] = (seq_t)base_terms_full[i];
    cache_len = 8;
}

/* Primary function exported by this file. */
seq_t a(idx_t n)
{
    init_cache();

    if ((size_t)n < cache_len) return cache[(size_t)n];

    /* Compute values up to n. */
    size_t target = (size_t)n;
    for (size_t i = cache_len; i <= target; ++i) {
        if (i < 8) continue;   /* already set */

        ensure_capacity(i + 1);

        /* numerator = Σ_{j=1..8} coeff(i,j) * a(i-j) */
        seq_t num = 0;
        for (int j = 1; j <= 8; ++j) {
            int64_t c = coeff((idx_t)i, j);
            seq_t coeff_t = (seq_t)c;
            num += coeff_t * cache[i - j];
        }

        /* denominator = 3*(i+1)*(i-2) – always positive for i≥8. */
        seq_t denom = (seq_t)1;
        denom *= 3;
        denom *= (i + 1);
        denom *= (i - 2);

        /* division is exact by the recurrence. */
        cache[i] = num / denom;
    }

    cache_len = target + 1;
    return cache[(size_t)n];
}
