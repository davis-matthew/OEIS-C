/*
 * A255258  Expansion of q^2 * phi(q) * psi(q^16) in powers of q where phi(),
 *          psi() are Ramanujan theta functions.
 * https://oeis.org/A255258
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in q^2 * phi(q) * psi(q^16)
 * Range:     u8 n <= 65, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static inline bool is_square(seq_t n)
{
    seq_t r = floor_sqrt(n);
    seq_t r2 = r * r;
    return r2 == n;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t shift = (seq_t)(n - 2);
    seq_t result = 0;

    for (idx_t j = 0; ; ++j) {
        seq_t s = (seq_t)8 * j * (j + 1);
        if (s > shift)
            break;

        seq_t e = shift - s;
        seq_t coeff_phi;

        if (e == 0)
            coeff_phi = 1;
        else if (is_square(e))
            coeff_phi = 2;
        else
            coeff_phi = 0;

        result += coeff_phi;
    }

    return result;
}
