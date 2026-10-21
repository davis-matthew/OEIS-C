/*
 * A264565  Number of (n+1) X (4+1) arrays of permutations of 0..n*5+4 with
 *          each element having directed index change 1,0 1,1 0,-1 or -1,1.
 * https://oeis.org/A264565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A264565
 *  Empirical recurrence for n > 7:
 *      a(n) = 18*a(n-2) + 36*a(n-3) - 45*a(n-4) - 216*a(n-5) - 243*a(n-6)
 *  Initial terms:
 *      a(1)=2, a(2)=8, a(3)=44, a(4)=192, a(5)=888,
 *      a(6)=4104, a(7)=18540
 *
 *  This function caches computed terms for efficiency.  It works
 *  for all n ≥ 1 and respects the integer type seq_t supplied by the
 *  build system.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

#define MAX_N 10000   /* maximum n we cache; sufficient for the benchmark */

static const seq_t init_terms[8] = {
    0, /* unused; a(0) not defined */
    2, /* a(1) */
    8, /* a(2) */
    44, /* a(3) */
    192, /* a(4) */
    888, /* a(5) */
    4104, /* a(6) */
    18540 /* a(7) */
};

static seq_t cache[MAX_N + 1];
static bool valid[MAX_N + 1];
static size_t cached_size = 0;

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Only support n ≥ 1; behaviour for n == 0 is unspecified */
    if (n == 0 || n > (idx_t)MAX_N) {
        /* Out of supported range; compute on the fly without caching */
        if (n <= 7) return init_terms[(size_t)n];

        /* temporary array for on‑the‑fly computation */
        seq_t *temp = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        for (size_t i = 1; i <= 7; ++i) temp[i] = init_terms[i];
        seq_t result = temp[(size_t)n];
        for (size_t i = 8; i <= n; ++i) {
            result = 18 * temp[i-2] + 36 * temp[i-3] - 45 * temp[i-4]
                     - 216 * temp[i-5] - 243 * temp[i-6];
            temp[i] = result;
        }
        free(temp);
        return result;
    }

    /* Fast cached path for n ≤ MAX_N */
    size_t idx = (size_t)n;

    /* Initialise cache if this is the first call */
    if (cached_size == 0) {
        for (size_t i = 1; i <= 7; ++i) {
            cache[i] = init_terms[i];
            valid[i] = true;
        }
        cached_size = 7;
    }

    /* Return cached value if available */
    if (valid[idx]) return cache[idx];

    /* Extend cache up to n */
    for (size_t i = cached_size + 1; i <= idx; ++i) {
        cache[i] = 18 * cache[i-2] + 36 * cache[i-3] - 45 * cache[i-4]
                   - 216 * cache[i-5] - 243 * cache[i-6];
        valid[i] = true;
    }
    cached_size = idx;

    return cache[idx];
}
