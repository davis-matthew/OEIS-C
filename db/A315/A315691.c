/*
 * A315691  Coordination sequence Gal.5.306.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A315691
 *  Coordination sequence Gal.5.306.5, offset 0.
 *
 *  Recurrence (for n > 10):
 *      a(n) =  2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5)
 *             - 2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10)
 *
 *  Initial values:
 *      a(0)=1, a(1)=6, a(2)=12, a(3)=17, a(4)=22, a(5)=28, a(6)=34,
 *      a(7)=39, a(8)=44, a(9)=50, a(10)=56.
 *
 *  The routine memoises past results.  Calls can arrive in any order.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

/* ---------- static cache ---------- */
static seq_t *cache = NULL;        /* array holding computed values   */
static size_t  cache_cap = 0;      /* currently allocated capacity     */
static size_t  computed = 0;       /* highest index already computed   */

/* initialise the cache with the first 11 values */
static void cache_init(void)
{
    const seq_t init[11] = {
        1, 6, 12, 17, 22, 28, 34, 39, 44, 50, 56
    };
    cache_cap = 11;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);   /* unrecoverable allocation failure */
    for (size_t i = 0; i < cache_cap; ++i)
        cache[i] = init[i];
    computed = 10;                    /* indices 0..10 are known          */
}

/* ensure the cache can hold at least indices 0..n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_init();
        return;
    }
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* compute a(n) using memoisation and the recurrence */
seq_t a(idx_t n)
{
    if (n < 0) return 0;   /* undefined, but avoid wrap‑around */
    ensure_cache(n);

    /* compute missing values up to n */
    if ((size_t)n > computed) {
        for (size_t i = computed + 1; i <= (size_t)n; ++i) {
            /* recurrence applies only for i >= 11 */
            seq_t val = 2 * cache[i - 1];
            val -= 2 * cache[i - 2];
            val += 2 * cache[i - 3];
            val -= 2 * cache[i - 4];
            val += 2 * cache[i - 5];
            val -= 2 * cache[i - 6];
            val += 2 * cache[i - 7];
            val -= 2 * cache[i - 8];
            val += 2 * cache[i - 9];
            val -= cache[i - 10];
            cache[i] = val;
        }
        computed = n;
    }

    return cache[(size_t)n];
}
