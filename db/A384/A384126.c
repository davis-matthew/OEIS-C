/*
 * A384126  Consecutive states of a linear congruential pseudo-random number
 *          generator (93*s+1) mod 2^13 when started at s=1.
 * https://oeis.org/A384126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (93*a(n-1) + 1) mod 2^13, a(1) = 1 The sequence has a
 *            period of 8192. This implementation precomputes the entire cycle
 *            once and answers any index in O(1)
 * Range:     u8 none, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t _a_cycle[8192];
static bool _a_initialized = false;

static void _a_init(void)
{
    if (_a_initialized) return;
    seq_t cur = 1;
    for (idx_t i = 0; i < 8192; ++i) {
        _a_cycle[i] = cur;
        cur = (93 * cur + 1) % (seq_t)8192;
    }
    _a_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n < 1, but return 0 safely */
    _a_init();
    idx_t pos = (n - 1) & 8191u; /* modulo 8192, since 8192 = 2^13 */
    return _a_cycle[pos];
}
