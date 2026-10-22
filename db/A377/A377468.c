/*
 * A377468  Least perfect-power >= n.
 * https://oeis.org/A377468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least perfect power >= n (i.e. the smallest m^k with
 *            integers m>1, k>1, that is ≥ n, where 1 is considered a perfect
 *            power)
 * Range:     u8 n <= 63, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   floor_sqrt, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n <= 1) return 1;               /* 1 is the first perfect power */

    const seq_t nval = (seq_t)n;
    seq_t best = 0;                    /* 0 indicates 'unset' */

    /* Scan all bases whose square is ≤ n.
       For each base we consider all higher exponents until the value reaches (or exceeds) n. */
    for (idx_t base = 2; base * base <= n; ++base) {
        seq_t val = pow_uint(base, 2);   /* base² */
        if (val >= nval) {
            if (best == 0 || val < best) best = val;
        }
        for (idx_t exp = 3; ; ++exp) {
            val = pow_uint(base, exp);
            if (val >= nval) {
                if (best == 0 || val < best) best = val;
                break;
            }
        }
    }

    /* Handle the perfect square that is the next integer after floor(√n). */
    seq_t baseLarge = floor_sqrt(nval) + 1;
    seq_t sq = baseLarge * baseLarge;
    if (sq >= nval) {
        if (best == 0 || sq < best) best = sq;
    }

    return best;
}
