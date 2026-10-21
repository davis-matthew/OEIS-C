/*
 * A284540  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 974", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A284540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 10000*a(n-4) - 10000*a(n-5) for n > 8 a(0)=1,
 *            a(1)=11, a(2)=111, a(3)=1101, a(4)=11101, a(5)=110101,
 *            a(6)=1110001, a(7)=11011011, a(8)=111011111
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


static seq_t *cache = NULL;
static size_t cache_count = 0;

static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_count)
        return;

    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp)
        abort();   /* memory allocation failed */

    cache = tmp;

    static const seq_t init[9] = {
        (seq_t)1,
        (seq_t)11,
        (seq_t)111,
        (seq_t)1101,
        (seq_t)11101,
        (seq_t)110101,
        (seq_t)1110001,
        (seq_t)11011011,
        (seq_t)111011111
    };

    for (size_t i = cache_count; i < needed; ++i) {
        if (i <= 8) {
            cache[i] = init[i];
        } else {
            cache[i] = cache[i-1] + (seq_t)10000 * cache[i-4]
                                   - (seq_t)10000 * cache[i-5];
        }
    }
    cache_count = needed;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
