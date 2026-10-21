/*
 * A278641  Number of pairs of orientable necklaces with n beads and up to 5
 *          colors; i.e., turning the necklace over does not leave it
 *          unchanged. The turned-over necklace is not included in the count.
 * https://oeis.org/A278641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -(5^floor((n+1)/2) + 5^ceil((n+1)/2)) / 4 * + (1/(2n)) *
 *            Σ_{d|n} φ(d) * 5^(n/d), k = 5 * a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

static seq_t pow5(idx_t e)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t aexp = (n + 1) / 2;
    idx_t bexp = (n + 2) / 2;

    seq_t pow_a = pow5(aexp);
    seq_t pow_b = pow5(bexp);

    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            seq_t phi_d = phi(d);
            idx_t exp = n / d;
            seq_t pow_val = pow5(exp);
            sum += phi_d * pow_val;
        }
    }

    seq_t term1 = sum / ((seq_t)n * 2);
    seq_t term2 = (pow_a + pow_b) / 4;

    return term1 - term2;
}
