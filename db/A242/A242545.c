/*
 * A242545  Number of length n+3 0..4 arrays with no four elements in a row
 *          with pattern aabb (possibly a=b) and new values 0..4 introduced in
 *          0..4 order.
 * https://oeis.org/A242545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 14*a(n-2) + 29*a(n-3) - 53*a(n-4) - 18*a(n-5)
 *            - 28*a(n-6) - 56*a(n-7) + 140*a(n-8) + 176*a(n-9) + 192*a(n-10)
 *            + 192*a(n-11) + 64*a(n-12), n ≥ 13 Initial values: a(1)=13,
 *            a(2)=43, a(3)=165, a(4)=690, a(5)=3050, a(6)=13988, a(7)=65588,
 *            a(8)=311431, a(9)=1489435, a(10)=7152787, a(1...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* cache[1] holds a(1) */
static size_t cache_cap = 0;     /* capacity in elements */
static idx_t  last_computed = 0; /* highest n with cached value */

/* Ensure cache has space for indices 0..n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;

    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) return; /* allocation failed – caller will get untouched entries */

    if (cache) {
        memcpy(tmp, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute terms up to n using the recurrence and store them in cache. */
static void compute_up_to(idx_t n)
{
    ensure_cache(n);
    if (n <= last_computed) return;

    /* Initial values for 1..12 */
    static const seq_t init_vals[13] = {
        0,              /* unused index 0 */
        13, 43, 165, 690,
        3050, 13988, 65588,
        311431, 1489435, 7152787,
        34431086, 165959996
    };

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 12) {
            cache[i] = init_vals[(int)i];
        } else {
            /* sum of positive coefficient terms */
            seq_t pos = (seq_t)7  * cache[i-1]
                      + (seq_t)29 * cache[i-3]
                      + (seq_t)140 * cache[i-8]
                      + (seq_t)176 * cache[i-9]
                      + (seq_t)192 * cache[i-10]
                      + (seq_t)192 * cache[i-11]
                      + (seq_t)64  * cache[i-12];

            /* sum of negative coefficient terms */
            seq_t neg = (seq_t)14 * cache[i-2]
                      + (seq_t)53 * cache[i-4]
                      + (seq_t)18 * cache[i-5]
                      + (seq_t)28 * cache[i-6]
                      + (seq_t)56 * cache[i-7];

            cache[i] = pos - neg;    /* unsigned subtraction is safe */
        }
    }
    last_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* sequence is defined for n ≥ 1 */
    compute_up_to(n);
    return cache[n];
}
