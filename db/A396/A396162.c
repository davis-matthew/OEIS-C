/*
 * A396162  First column of the triangular array with T(0, m) = m^m and T(n,
 *          m) = T(n - 1, m + 3) - 2*T(n - 1, m + 2) + T(n - 1, m + 1).
 * https://oeis.org/A396162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{2n} (-1)^k · binomial(2n, k) · (n+k)^(n+k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t two_n = 2 * n;
    seq_t pos = 0;
    seq_t neg = 0;
    for (idx_t k = 0; k <= two_n; ++k) {
        seq_t coeff = binomial(two_n, k);
        seq_t power = big_pow((seq_t)(n + k), (seq_t)(n + k));
        seq_t term  = coeff * power;
        if ((k & 1) == 0)
            pos += term;
        else
            neg += term;
    }
    return pos - neg;
}
