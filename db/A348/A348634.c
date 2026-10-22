/*
 * A348634  Number of transitive relations on an n-set with exactly five
 *          ordered pairs.
 * https://oeis.org/A348634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*C(n,3) + 660*C(n,4) + 5201*C(n,5) + 21822*C(n,6) +
 *            54600*C(n,7) + 84000*C(n,8) + 75600*C(n,9) + 30240*C(n,10)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 16, u64 n <= 138, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static const idx_t coeffs[11] = {
        0ULL,                 // C(n,0)
        0ULL,                 // C(n,1)
        0ULL,                 // C(n,2)
        27ULL,                // C(n,3)
        660ULL,               // C(n,4)
        5201ULL,              // C(n,5)
        21822ULL,             // C(n,6)
        54600ULL,             // C(n,7)
        84000ULL,             // C(n,8)
        75600ULL,             // C(n,9)
        30240ULL              // C(n,10)
    };

    seq_t result = 0;
    for (idx_t k = 3; k <= 10 && k <= n; ++k) {
        seq_t bin = binomial(n, k);
        result += (seq_t)coeffs[k] * bin;
    }
    return result;
}
