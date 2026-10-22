/*
 * A398912  Triangle read by rows: Number of functions f: [n] -> [n] where the
 *          longest nondecreasing initial run has length k.
 * https://oeis.org/A398912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = the triangular array read by rows. Determine the row m
 *            and column k from n, then use the closed form for T(m,k): if k
 *            == m : T = binomial(2*m-1, m) else if k == 0 : T = 0 else : T =
 *            k * binomial(m + k - 1, k + 1) * m^(m-k-1) with T(0,0) = 1
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 55, u64 n <= 153,
 *            u128 n <= 406, big any n
 * Helpers:   _floor_isqrt_u64, big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"   /* for _floor_isqrt_u64 */
#include "../utils/binomial.h"          /* for binomial() */
#include "../utils/big_pow.h"           /* for big_pow() */

static inline idx_t
row_of(idx_t n) {
    /* Compute m such that m(m+1)/2 <= n < (m+1)(m+2)/2
       m = floor((sqrt(8*n+1)-1)/2) */
    idx_t s = _floor_isqrt_u64(8*n + 1);
    return (s - 1) / 2;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* T(0,0) */

    idx_t m = row_of(n);
    idx_t k = n - m*(m + 1)/2;

    /* Handle the case k == m (last element of the row) */
    if (k == m) {
        return binomial(2*m - 1, m);
    }

    /* k == 0 gives 0 unless m == 0, which is handled above */
    if (k == 0) {
        return (seq_t)0;
    }

    /* 0 < k < m */
    seq_t term = (seq_t)k;
    term *= binomial(m + k - 1, k + 1);
    term *= big_pow((seq_t)m, (seq_t)(m - k - 1));
    return term;
}
