/*
 * A283897  Relative of Hofstadter Q-sequence: a(-90) = 91, a(-89) = 2;
 *          thereafter a(n) = a(n-a(n-1)) + a(n-a(n-2)).
 * https://oeis.org/A283897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)); initial values: a(-90) = 91,
 *            a(-89) = 2; convention: a(n) = 0 for n <= -91; a(1) is the first
 *            term listed
 * Range:     u8 none, u16 none, u32 none, u64 n <= 375, u128 n <= 375,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined only up to n = 375; for larger n a(n) = 0. */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>   /* for memset */

typedef int64_t   sidx_t;   /* signed index type */

/* Size of the cache: indices from -90 to 375 inclusive   */
#define CACHE_SIZE   466   /* 375 - (-90) + 1 */

/* Static cache for computed terms and a flag array. */
static seq_t  cache[CACHE_SIZE];
static bool   computed[CACHE_SIZE];
static bool   cache_inited = false;

/* Initialise the computed flag array once. */
static void init_cache(void)
{
    if (!cache_inited) {
        memset(computed, 0, sizeof(computed));
        cache_inited = true;
    }
}

/* Recursive computation for signed indices. */
static seq_t compute_sidx(sidx_t n)
{
    /* Convention: a(n) = 0 for n <= -91. */
    if (n <= -91) return (seq_t)0;

    /* The sequence has defined terms only up to 375. */
    if (n > 375) return (seq_t)0;

    init_cache();

    size_t idx = (size_t)(n + 90);   /* map [-90 .. 375] to [0 .. 465] */

    if (computed[idx]) return cache[idx];

    /* Handle the two seed values. */
    if (n == -90) {
        cache[idx] = (seq_t)91;
        computed[idx] = true;
        return cache[idx];
    }
    if (n == -89) {
        cache[idx] = (seq_t)2;
        computed[idx] = true;
        return cache[idx];
    }

    /* Recursively obtain a(n-1) and a(n-2). */
    seq_t an1 = compute_sidx(n - 1);
    seq_t an2 = compute_sidx(n - 2);

    /* Determine the two recursive arguments. */
    /* If an1 (or an2) is large enough that n-an1 <= -91, the value
       is 0 by convention. */
    seq_t term1, term2;

    if (an1 > (seq_t)(n + 91)) {
        term1 = (seq_t)0;
    } else {
        sidx_t arg1 = (sidx_t)( (seq_t)n - an1 );
        term1 = compute_sidx(arg1);
    }

    if (an2 > (seq_t)(n + 91)) {
        term2 = (seq_t)0;
    } else {
        sidx_t arg2 = (sidx_t)( (seq_t)n - an2 );
        term2 = compute_sidx(arg2);
    }

    seq_t result = term1 + term2;

    cache[idx] = result;
    computed[idx] = true;

    return result;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* outside the defined range */
    if (n > 375) return (seq_t)0;              /* beyond the last non‑zero term */
    return compute_sidx((sidx_t)n);
}
