/*
 * A268070  Number of nX2 0..3 arrays with every repeated value in every row
 *          and column unequal to the previous repeated value, and new values
 *          introduced in row-major sequential order.
 * https://oeis.org/A268070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A268070.
 *
 * Empirical recurrence (valid for n > 13):
 *   a(n) = 26*a(n-1) -109*a(n-2) -1668*a(n-3) +10384*a(n-4) +30340*a(n-5)
 *          -193268*a(n-6) -33480*a(n-7) +882432*a(n-8) -461376*a(n-9)
 *          -1073088*a(n-10) +839808*a(n-11)
 *
 * Initial values (1‑based):
 *   a(1) = 2
 *   a(2) = 15
 *   a(3) = 159
 *   a(4) = 2191
 *   a(5) = 30295
 *   a(6) = 421471
 *   a(7) = 5797959
 *   a(8) = 79367143
 *   a(9) = 1079774311
 *   a(10) = 14630901175
 *   a(11) = 197520668295
 *   a(12) = 2659138662775
 *   a(13) = 35714778101863
 *
 * The function memoises results so that any order of calls is supported.
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Initial terms (1‑based index 1..13). */
static const seq_t init_vals[13] = {
    2ULL,      15ULL,      159ULL,      2191ULL,      30295ULL,
    421471ULL, 5797959ULL, 79367143ULL, 1079774311ULL, 14630901175ULL,
    197520668295ULL, 2659138662775ULL, 35714778101863ULL
};

/* Coefficients for the 11‑term recurrence (index 0 = a(n-1)). */
static const int coeffs[11] = {
    26,   -109,   -1668,  10384,   30340,
    -193268, -33480, 882432, -461376, -1073088, 839808
};

/* Cached sequence (1‑based in comments, 0‑based in array). */
static seq_t *cache      = NULL;   /* Pointer to allocated array. */
static size_t cache_cap  = 0;       /* Current allocation in entries. */
static idx_t   cache_len = 0;       /* Number of terms stored (>= 0). */

/* Ensure the cache can hold at least `n` entries. */
static void ensure_capacity(idx_t n)
{
    if (cache_cap >= (size_t)n)
        return;
    size_t new_cap = cache_cap < 64 ? 64 : cache_cap * 2;
    while (new_cap < (size_t)n)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return;                    /* Allocation failure – unlikely. */
    if (cache && cache_len > 0) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the cache with the first 13 terms. */
static void init_cache(void)
{
    if (cache_len != 0)
        return;                    /* Already initialised. */
    ensure_capacity(13);
    memcpy(cache, init_vals, 13 * sizeof(seq_t));
    cache_len = 13;
}

/* Compute terms up to n (1‑based).  Extends the cache as necessary. */
static void compute_up_to(idx_t n)
{
    init_cache();                  /* Guarantees 13 terms available. */
    if (cache_len >= n)
        return;                    /* Nothing to compute. */
    ensure_capacity(n);            /* Ensure we have enough space. */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t term = 0;
        /* i is the 0‑based index of the term to compute (a(i+1)). */
        for (int k = 0; k < 11; ++k) {
            seq_t prev = cache[i - 1 - k];   /* a(i-1-k+1) = a(i-k). */
            int coeff = coeffs[k];
            if (coeff >= 0)
                term += prev * (seq_t)coeff;
            else
                term -= prev * (seq_t)(-coeff);
        }
        cache[i] = term;
    }
    cache_len = n;
}

/* Public sequence entry point. */
seq_t a(idx_t n)
{
    if (n <= 13)
        return init_vals[n - 1];
    compute_up_to(n);
    return cache[n - 1];
}
