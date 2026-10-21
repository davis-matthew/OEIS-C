/*
 * A266838  Binary representation of the n-th iteration of the "Rule 67"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11001*a(n-2) - 10011000*a(n-4) + 10000000*a(n-6) for n >
 *            9; a(0) = 1, a(1) = 100, a(2) = 1010, a(3) = 1100001, a(4) =
 *            1011100, a(5) = 11100001011, a(6) = 1011100000, a(7) =
 *            111100001011111, a(8) = 1011100000000, a(9) =
 *            1111100001011111111
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[10] = {
    1ULL,
    100ULL,
    1010ULL,
    1100001ULL,
    1011100ULL,
    11100001011ULL,
    1011100000ULL,
    111100001011111ULL,
    1011100000000ULL,
    1111100001011111111ULL
};

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n < 10) return init[(size_t)n];

    size_t N = (size_t)n;
    if (N + 1 > cache_len) {
        size_t newlen = N + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – return 0 */
        cache = tmp;
        for (size_t i = cache_len; i < newlen; ++i) {
            if (i < 10) {
                cache[i] = init[i];
            } else {
                cache[i] = (cache[i - 2] * 11001ULL)
                         - (cache[i - 4] * 10011000ULL)
                         + (cache[i - 6] * 10000000ULL);
            }
        }
        cache_len = newlen;
    }
    return cache[N];
}
