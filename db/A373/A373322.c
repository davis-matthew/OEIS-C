/*
 * A373322  The number of indecomposable summands, counted with multiplicity,
 *          in tensor powers of the vector representation of SL2 in
 *          characteristic 2.
 * https://oeis.org/A373322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1. For n > 1: a(2n-1) = a(2n) = Σ_{k=0}^{n-1}
 *            binom(n-1,k) * 2^(n-1-k) * a(k)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 36, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- static storage ---------- */
static seq_t *cache_array = 0;      /* memoised a(n) */
static size_t cache_cap   = 0;      /* allocated capacity of cache_array */
static idx_t  cache_last  = 0;      /* highest index known (0‑based) */
static size_t next_n      = 2;      /* next n to process (>=2) */

static seq_t *pow2_array = 0;      /* cached powers of two 2^e, e = 0.. */
static size_t pow2_cap   = 0;      /* allocated capacity of pow2_array */

static bool initialized = false;

/* ---------- helper: grow cache_array to at least 'need' elements ---------- */
static void ensure_cache_capacity(size_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache_array, new_cap * sizeof(seq_t));
    cache_array = tmp;
    cache_cap   = new_cap;
}

/* ---------- helper: grow pow2_array to at least exponent 'e' ---------- */
static void ensure_pow2_up_to(size_t e) {
    if (e < pow2_cap) return;
    size_t new_cap = pow2_cap ? pow2_cap : 1;
    while (new_cap <= e) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(pow2_array, new_cap * sizeof(seq_t));
    pow2_array = tmp;
    if (pow2_cap == 0) {                 /* first allocation */
        pow2_array[0] = (seq_t)1;
        for (size_t i = 1; i < new_cap; ++i) {
            pow2_array[i] = pow2_array[i-1] * ((seq_t)2);
        }
    } else {
        for (size_t i = pow2_cap; i < new_cap; ++i) {
            pow2_array[i] = pow2_array[i-1] * ((seq_t)2);
        }
    }
    pow2_cap = new_cap;
}

/* ---------- compute a(n) values up to index 'n' (inclusive) ---------- */
static void compute_up_to(idx_t n) {
    if ((idx_t)cache_last >= n) return;            /* already up‑to‑date */

    /* Need to compute all a(2m-1)=a(2m) for m = next_n … ceil(n/2) */
    size_t target_m = (size_t)((n + 1) / 2);        /* ceil(n/2) */

    while (next_n <= target_m) {
        size_t m = next_n;

        /* ensure storage */
        ensure_cache_capacity(2 * m + 1);        /* indices up to 2m */
        ensure_pow2_up_to(m - 1);                /* need 2^(m-1-k) */

        seq_t sum = 0;
        seq_t C   = 1;                           /* binom(m-1,0) */

        for (size_t k = 0; k < m; ++k) {
            seq_t term = cache_array[k];
            term *= C;
            term *= pow2_array[(m - 1) - k];
            sum += term;

            if (k + 1 < m) {
                C *= (seq_t)(m - 1 - k);
                C /= (seq_t)(k + 1);
            }
        }

        cache_array[2 * m - 1] = sum;
        cache_array[2 * m]     = sum;

        if ((idx_t)(2 * m) > cache_last) cache_last = (idx_t)(2 * m);
        next_n++;
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n) {
    if (!initialized) {
        /* initial cache with a(0)=a(1)=1, a(2)=1 */
        cache_cap   = 8;
        cache_array = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_array[0] = (seq_t)1;
        cache_array[1] = (seq_t)1;
        cache_array[2] = (seq_t)1;
        cache_last    = 2;          /* highest index known */

        /* pow2_array with 2^0 */
        pow2_cap   = 1;
        pow2_array = (seq_t *)malloc(pow2_cap * sizeof(seq_t));
        pow2_array[0] = (seq_t)1;

        initialized = true;
    }

    compute_up_to(n);
    return cache_array[(size_t)n];
}
