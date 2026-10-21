/*
 * A298803  Growth series for group with presentation < S, T : S^3 = T^3 =
 *          (S*T)^4 = 1 >.
 * https://oeis.org/A298803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 3*a(n-3) + a(n-4) - a(n-6) (n > 7) Initial
 *            values: a(0)=1, a(1)=4, a(2)=8, a(3)=16, a(4)=30, a(5)=50,
 *            a(6)=88, a(7)=150 (Growth series for group
 *            <S,T|S^3=T^3=(ST)^4=1>)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 38, u64 n <= 79, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Cached terms -------------------------------------------------------- */
static seq_t *cache       = NULL; /* dynamic array of terms */
static size_t cache_cap   = 0;    /* allocated capacity, in elements */
static size_t last_computed = SIZE_MAX; /* largest index computed */

/* --------------------------------------------------------------------- */
static void init_cache(void)
{
    /* Allocate space for the first 8 terms */
    size_t needed = 8;
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (cache == NULL) return;   /* allocation failure -- will crash later */
        cache_cap = needed;
    }

    /* Initial values -------------------------------------------------- */
    static const seq_t init_vals[8] = {
        (seq_t)1,  (seq_t)4,  (seq_t)8,
        (seq_t)16, (seq_t)30, (seq_t)50,
        (seq_t)88, (seq_t)150
    };
    memcpy(cache, init_vals, sizeof(init_vals));

    last_computed = 7;
}

/* --------------------------------------------------------------------- */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) init_cache();   /* lazy initialization */

    size_t required = (size_t)n + 1;
    if (required <= cache_cap) return; /* already large enough */

    /* Grow capacity exponentially */
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < required) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (new_cache == NULL) return; /* allocation failure */

    memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
    free(cache);
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* If already computed, just return the stored value */
    if ((size_t)n <= last_computed)
        return cache[(size_t)n];

    /* Compute missing terms using the recurrence */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        seq_t val = cache[i - 2];
        val = val + (seq_t)3 * cache[i - 3];
        val = val + cache[i - 4];
        val = val - cache[i - 6];
        cache[i] = val;
    }

    last_computed = (size_t)n;
    return cache[(size_t)n];
}
