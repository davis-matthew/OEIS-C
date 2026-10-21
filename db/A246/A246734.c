/*
 * A246734  Number of length n+4 0..5 arrays with no pair in any consecutive
 *          five terms totalling exactly 5.
 * https://oeis.org/A246734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 12, u64 n <= 30, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) + a(n-2) + a(n-3) + 5*a(n-4) + a(n-5) - a(n-6) - a(n-7)
 * Initial values (offset 1):
 * a(1)=1566, a(2)=5430, a(3)=18966, a(4)=66294, a(5)=231414,
 * a(6)=807630, a(7)=2818830
 */
#include <stdlib.h>

static seq_t *cache = NULL;            /* array of computed values, indexed by 1.. */
static idx_t cache_cap = 0;            /* capacity of the cache array */
static idx_t max_computed = 0;         /* highest n for which a(n) is already stored */

/* Ensure the cache has room for indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    idx_t new_cap = cache_cap > 0 ? cache_cap : 8;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Out of memory; abort. In contest environments this is unlikely. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Fill the cache with the first seven values. */
static void init_base(void)
{
    if (max_computed >= 7)
        return;               /* already initialized */
    ensure_cache_size(7);
    cache[0] = 0;              /* unused; indices start at 1 */
    cache[1] = 1566;
    cache[2] = 5430;
    cache[3] = 18966;
    cache[4] = 66294;
    cache[5] = 231414;
    cache[6] = 807630;
    cache[7] = 2818830;
    max_computed = 7;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* undefined for n < 1, return 0 */
    init_base();

    if (n <= max_computed)
        return cache[n];

    ensure_cache_size(n);
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* recurrence: a[i] = 3*a[i-1] + a[i-2] + a[i-3] + 5*a[i-4] + a[i-5]
         *               - a[i-6] - a[i-7];
         * All operations are on unsigned seq_t; subtraction will not overflow
         * because the true result is non‑negative. */
        seq_t val = 3 * cache[i-1];
        val += cache[i-2];
        val += cache[i-3];
        val += 5 * cache[i-4];
        val += cache[i-5];
        val -= cache[i-6];
        val -= cache[i-7];
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
