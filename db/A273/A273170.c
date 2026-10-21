/*
 * A273170  Denominators of coefficient triangle for integrated even powers of
 *          cos(x) in terms of x and sin(2*m*x).
 * https://oeis.org/A273170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac, easy
 * Formula:   a(n) = denominator of the rational * R(row, col) =
 *            binomial(2*row, row-col)/(col*2^(2*row)) * where (row, col) are
 *            determined by the triangular indexing of n: * n = 0, 1, 2, 3, 4,
 *            5, 6, ... * (row, col) = (0,0), (1,0), (1,1), (2,0), (2,1),
 *            (2,2), (3,0), ... * * The denominator is calculated as (2...
 * Range:     u8 n <= 13, u16 n <= 32, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, gcd, pow2, v2_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/pow2.h"
#include "../utils/binomial.h"
#include "../utils/v2_seq.h"

seq_t a(idx_t n) {
    /* Find the row (r) such that n lies in that row of the triangle */
    idx_t r = 0;
    /* Use 128‑bit arithmetic to avoid overflow while testing bounds */
    while (( (__uint128_t)(r + 1) * (r + 2) ) / 2 <= n) {
        ++r;
    }
    /* Column inside the row */
    idx_t col = n - ( (__uint128_t)r * (r + 1) ) / 2; /* 0 ≤ col ≤ r */

    /* Special handling for col == 0: treat as multiplier 1 */
    seq_t mult = (col == 0) ? (seq_t)1 : (seq_t)col;

    /* Binomial coefficient */
    seq_t b = binomial(2 * r, r - col);

    /* Extract powers of two */
    int v2_b = v2_seq(b);                     /* exponent of 2 in b      */
    int v2_col = (col == 0) ? 0 : __builtin_ctzll(col); /* exponent of 2 in col */

    /* Total exponent of two in the numerator a = 2^(2r) * col   */
    int exp2_total = 2 * r + v2_col;

    /* Exponent of two in the gcd */
    int g_twos_exp = (v2_b < exp2_total) ? v2_b : exp2_total;

    /* Remaining odd part of a after removing twos */
    seq_t col_odd = (col == 0) ? (seq_t)1 : (seq_t)(col >> v2_col);

    /* Odd part of b after removing twos */
    seq_t b_odd = b >> v2_b;

    /* Odd part of the gcd */
    seq_t odd_gcd = gcd(b_odd, col_odd);

    /* Final power of two exponent for the denominator */
    int odd_exp = exp2_total - g_twos_exp;

    /* Compute denominator: 2^(odd_exp) * (col_odd / odd_gcd) */
    seq_t denom = pow2(odd_exp);
    denom = denom * (col_odd / odd_gcd);

    return denom;
}
