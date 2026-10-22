/*
 * A331333  Interpolating the factorial and the powers of 2. Triangle read by
 *          rows, T(n, k) for 0 <= k <= n.
 * https://oeis.org/A331333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k), where m = floor((sqrt(8*n+1)-1)/2), k = n -
 *            m*(m+1)/2, and T(m,k) = 2^k * (m!/k!) * C(m,k)
 * Range:     u8 n <= 11, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static idx_t find_row(idx_t n) {
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    return r;
}

seq_t a(idx_t n) {
    idx_t r = find_row(n);
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;

    seq_t result = pow2(k);
    result *= binomial(r, k);

    for (idx_t i = k + 1; i <= r; ++i) {
        result *= (seq_t)i;
    }
    return result;
}
