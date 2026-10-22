/*
 * A316547  Number of nX3 0..1 arrays with every element unequal to 0, 1, 3,
 *          4, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence a(1)=3, a(2)=7, a(3)=10,
 *            a(4)=17, a(5)=36, a(6)=69, a(7)=129, a(8)=260, a(9)=544,
 *            a(10)=1125, a(11)=2348, a(12)=4964, a(13)=10600, a(14)=22871,
 *            a(15)=49765, for n≥16: a(n) = 3*a(n‑1) – 4*a(n‑3) + 2*a(n‑4) –
 *            3*a(n‑5) – 4*a(n‑7) – 5*a(n‑8) + 17*a(n‑9) + 9*a(n‑10) –...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 56, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_capacity = 0;

/* Base values for n = 1..15 */
static const seq_t base_vals[] = {
    (seq_t)3, (seq_t)7, (seq_t)10, (seq_t)17, (seq_t)36,
    (seq_t)69, (seq_t)129, (seq_t)260, (seq_t)544, (seq_t)1125,
    (seq_t)2348, (seq_t)4964, (seq_t)10600, (seq_t)22871, (seq_t)49765
};
static const size_t BASE_LEN = sizeof base_vals / sizeof base_vals[0];

/* Ensure the cache is large enough for the first n terms and that
   the base values are stored. */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n;
    if (cache_capacity < needed) {
        size_t new_cap = cache_capacity ? cache_capacity << 1 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;               /* allocation failure – keep old cache */
        cache = tmp;
        cache_capacity = new_cap;
    }
    /* Fill base values if not yet initialized. */
    if (cache && cache[0] == 0) {
        for (size_t i = 0; i < BASE_LEN && i < cache_capacity; ++i)
            cache[i] = base_vals[i];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1 */

    ensure_cache_size(n);

    /* Return directly if value already known (including base). */
    if (n <= BASE_LEN) return cache[(size_t)n - 1];

    /* Compute missing terms up to n. */
    for (idx_t i = BASE_LEN; i < n; ++i) {
        seq_t val =
            3 * cache[(size_t)i - 1]
          - 4 * cache[(size_t)i - 3]
          + 2 * cache[(size_t)i - 4]
          - 3 * cache[(size_t)i - 5]
          - 4 * cache[(size_t)i - 7]
          - 5 * cache[(size_t)i - 8]
          + 17 * cache[(size_t)i - 9]
          + 9 * cache[(size_t)i - 10]
          - cache[(size_t)i - 11]
          - 3 * cache[(size_t)i - 12]
          - 15 * cache[(size_t)i - 13]
          + 4 * cache[(size_t)i - 14]
          + 3 * cache[(size_t)i - 15];
        cache[(size_t)i] = val;
    }
    return cache[(size_t)n - 1];
}
