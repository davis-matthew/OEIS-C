/*
 * A376827  T(n, k) = binomial(n, k)*hypergeom([(1 - n)/2, -n/2], [1], 4).
 * https://oeis.org/A376827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the triangle T where * T(r,k) =
 *            binomial(r,k) * A002426(r). * The mapping from index n to row r
 *            and column k is: * r = max{ m | m(m+1)/2 ≤ n } * k = n -
 *            r(r+1)/2 * * A002426(r) is computed by the identity * A002426(r)
 *            = Σ_{j=0}^{⌊r/2⌋} binomial(r,2j) * binomial(2j,j)
 * Range:     u8 n <= 16, u16 n <= 39, u32 n <= 124, u64 n <= 415,
 *            u128 n <= 1451, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static seq_t compute_A002426(idx_t n) {
    /* Cache up to a fixed maximum (row 200 > 10000 indices needed) */
    static seq_t cache[201];
    static idx_t cached_up_to = 0;

    if (n < cached_up_to) {
        return cache[n];
    }

    for (idx_t i = cached_up_to; i <= n; ++i) {
        seq_t sum = 0;
        idx_t maxj = i / 2;
        for (idx_t j = 0; j <= maxj; ++j) {
            seq_t part = binomial(i, 2 * j) * binomial(2 * j, j);
            sum += part;
        }
        cache[i] = sum;
    }
    cached_up_to = n + 1;
    return cache[n];
}

seq_t a(idx_t n) {
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    seq_t bin = binomial(r, k);
    seq_t val = compute_A002426(r);
    return bin * val;
}
