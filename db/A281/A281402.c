/*
 * A281402  Number of 3 X n 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A281402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 32, u64 n <= 72, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Empirical recurrence A281402:
 *   a(n) = 4*a(n-1) - 6*a(n-2) + 8*a(n-3) - 13*a(n-4)
 *               + 12*a(n-5) - 8*a(n-6) + 8*a(n-7) - 4*a(n-8)   for n > 13
 * Initial values (a(1)…a(13)):
 *   0, 6, 131, 314, 689, 1294, 2364, 4338, 7850,
 *   14150, 25510, 45790, 81958.
 *
 * The array is cached so the function works in any call order.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t _a_cache[10001];          /* holds values up to 10000 */
static idx_t _a_cache_max = 0;         /* highest index computed so far */
static bool _a_cache_init = false;     /* whether initial values are set */

seq_t a(idx_t n)
{
    /* n must be >= 1; behaviour for n < 1 is irrelevant */
    if (n == 0) return (seq_t)0;

    /* initialise the base values only once */
    if (!_a_cache_init) {
        _a_cache[1] = (seq_t)0;
        _a_cache[2] = (seq_t)6;
        _a_cache[3] = (seq_t)131;
        _a_cache[4] = (seq_t)314;
        _a_cache[5] = (seq_t)689;
        _a_cache[6] = (seq_t)1294;
        _a_cache[7] = (seq_t)2364;
        _a_cache[8] = (seq_t)4338;
        _a_cache[9] = (seq_t)7850;
        _a_cache[10] = (seq_t)14150;
        _a_cache[11] = (seq_t)25510;
        _a_cache[12] = (seq_t)45790;
        _a_cache[13] = (seq_t)81958;
        _a_cache_max = 13;
        _a_cache_init = true;
    }

    if (n <= _a_cache_max) return _a_cache[n];

    /* compute missing terms up to n */
    for (idx_t i = _a_cache_max + 1; i <= n; ++i) {
        seq_t v = 4 * _a_cache[i-1];
        v -= 6 * _a_cache[i-2];
        v += 8 * _a_cache[i-3];
        v -= 13 * _a_cache[i-4];
        v += 12 * _a_cache[i-5];
        v -= 8 * _a_cache[i-6];
        v += 8 * _a_cache[i-7];
        v -= 4 * _a_cache[i-8];
        _a_cache[i] = v;
    }

    _a_cache_max = n;
    return _a_cache[n];
}
