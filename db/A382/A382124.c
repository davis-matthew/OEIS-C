/*
 * A382124  G.f. A(x) = exp( Sum_{n>=1} sigma(n)*sigma(2*n)/3 * x^n/n ), where
 *          sigma(n) = A000203(n) is the sum of the divisors of n.
 * https://oeis.org/A382124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = (1/n) * \sum_{k=1..n} sigma(k)*sigma(2*k)/3 *
 *            a(n-k) (sigma is the sum‑of‑divisors function)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 37, u64 n <= 94, u128 n <= 243,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function uses O(n^2) recurrence; intermediate 128‑bit arithmetic
    is employed to avoid overflow for the 64‑bit build.  For C++ builds
    seq_t may be an arbitrary‑precision type; normal operator * / + /
    are used. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Local caches ----------------------------------------------------------- */
static seq_t *a_cache = NULL;      /* a[0..max_computed]   */
static size_t a_max_computed = 0;  /* largest n with a_cache[n] valid   */
static size_t a_need_cap = 0;      /* allocated capacity of a_cache       */

static seq_t *sigma_cache = NULL;  /* sigma[0..max_sigma]    */
static size_t sigma_max_computed = 0;  /* largest index filled   */
static size_t sigma_need_cap = 0;      /* allocated capacity of sigma_cache */

/* Ensure a_cache has room for indices up to max_n (inclusive). */
static void ensure_a_cache(size_t max_n)
{
    if (max_n + 1 > a_need_cap) {
        size_t new_cap = a_need_cap ? a_need_cap : 8;
        while (new_cap < max_n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache = tmp;
        a_need_cap = new_cap;
    }
}

/* Recompute sigma[1..limit] by a divisor‑sum sieve. */
static void recompute_sigma(size_t limit)
{
    if (limit + 1 > sigma_need_cap) {
        size_t new_cap = sigma_need_cap ? sigma_need_cap : 8;
        while (new_cap < limit + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(sigma_cache, new_cap * sizeof(seq_t));
        sigma_cache = tmp;
        sigma_need_cap = new_cap;
    }
    /* Zero initial array. */
    for (size_t i = 0; i <= limit; ++i)
        sigma_cache[i] = (seq_t)0;

    for (size_t d = 1; d <= limit; ++d) {
        seq_t val = (seq_t)d;
        for (size_t m = d; m <= limit; m += d)
            sigma_cache[m] += val;
    }
    sigma_max_computed = limit;
}

/* Ensure sigma[0..max_needed] is available. */
static void ensure_sigma(size_t max_needed)
{
    if (max_needed > sigma_max_computed)
        recompute_sigma(max_needed);
}

/* Main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    const size_t N = (size_t)n;

    /* Initialise a_cache on first call. */
    if (a_cache == NULL) {
        ensure_a_cache(1);
        a_cache[0] = (seq_t)1;
        a_max_computed = 0;
    }

    if (N == 0) return a_cache[0];

    /* Prepare sigma values up to 2*N. */
    ensure_sigma(2 * N);

    /* Compute terms up to N if not yet available. */
    if (N > a_max_computed) {
        ensure_a_cache(N);

        for (size_t i = a_max_computed + 1; i <= N; ++i) {
#if defined(__cplusplus)
            /* C++ arbitrary‑precision path */
            seq_t sum = 0;
            for (size_t k = 1; k <= i; ++k) {
                seq_t prod = sigma_cache[k] * sigma_cache[2 * k];
                prod *= a_cache[i - k];
                sum += prod;
            }
            seq_t denom = (seq_t)3 * (seq_t)i;
            a_cache[i] = sum / denom;
#else
            /* C 64‑bit path – use 128‑bit intermediates. */
            unsigned __int128 sum = 0;
            for (size_t k = 1; k <= i; ++k) {
                unsigned __int128 prod = (unsigned __int128)sigma_cache[k]
                                          * sigma_cache[2 * k];
                prod *= a_cache[i - k];
                sum += prod;
            }
            sum /= ((unsigned __int128)3 * (unsigned __int128)i);
            a_cache[i] = (seq_t)sum;
#endif
        }
        a_max_computed = N;
    }

    return a_cache[N];
}
