/*
 * A296134  Number of twice-factorizations of n of type (R,Q,R).
 * https://oeis.org/A296134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d | A052409(n)} A000009(d) where A052409(n) is the gcd
 *            of the exponents in the prime factorization of n
 * Range:     u8 n <= 9135, u16 n <= 9135, u32 n <= 9135, u64 n <= 9135,
 *            u128 n <= 9135, big any n
 * Helpers:   _gcd_int, _gcd_of_exponents
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */
#include <stdbool.h>
#include "../utils/_gcd_int.h"
#include "../utils/_gcd_of_exponents.h"

static seq_t _g0[65];
static int   _g0_init = 0;

/* Strict partition numbers A000009 up to 64 */
static void _init_g0(void)
{
    if (_g0_init) return;
    _g0[0] = (seq_t)1;
    for (int i = 1; i <= 64; ++i) _g0[i] = (seq_t)0;
    for (int k = 1; k <= 64; ++k) {
        for (int i = 64; i >= k; --i) {
            _g0[i] += _g0[i - k];
        }
    }
    _g0_init = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    _init_g0();
    int g = _gcd_of_exponents(n);

    seq_t result = 0;
    for (int d = 1; d <= g; ++d) {
        if (g % d == 0) {
            result += _g0[d];
        }
    }
    return result;
}
