/*
 * A283583  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 646", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 256*a(n-8) - 256*a(n-10) for n>11 with initial
 *            values: a(0)=1, a(1)=3, a(2)=5, a(3)=13, a(4)=21, a(5)=53,
 *            a(6)=81, a(7)=219, a(8)=321, a(9)=867, a(10)=1349, a(11)=3437
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();
    cache = new_ptr;
    cache_cap = new_cap;
}

static void init_base(void)
{
    static const seq_t base_vals[] = {
        (seq_t)1,  (seq_t)3,  (seq_t)5,  (seq_t)13, (seq_t)21, (seq_t)53,
        (seq_t)81, (seq_t)219, (seq_t)321, (seq_t)867, (seq_t)1349, (seq_t)3437
    };
    for (size_t i = 0; i < 12; ++i)
        cache[i] = base_vals[i];
    cache_len = 12;
}

seq_t a(idx_t n)
{
    if (cache == NULL) {
        ensure_cap(16);
        init_base();
    }

    if ((idx_t)cache_len > n)
        return cache[n];

    ensure_cap((size_t)n + 1);
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i <= 11) {
            val = cache[i];
        } else {
            seq_t t1 = cache[i - 2];
            seq_t t2 = cache[i - 8];
            seq_t t3 = cache[i - 10];
            t2 <<= 8;   /* multiply by 256 */
            t3 <<= 8;   /* multiply by 256 */
            val = t1 + t2 - t3;
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
