/*
 * A369311  Square array A(n, k), n >= 0, k > 0, read by upwards
 *          antidiagonals: P(A(n, k)) is the remainder of the polynomial long
 *          division of P(n) by P(k) (where P(m) denotes the polynomial over
 *          GF(2) whose coefficients are encoded in the binary expansion of
 *          the nonnegative integer m).
 * https://oeis.org/A369311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A369311
   The value a(n) is obtained by reading the square array
   A(n,k) = remainder of P(n) divided by P(k) over GF(2),
   where n ≥ 0, k > 0, and P(m) is the binary‐polynomial of m.
   The antidiagonal reading starts with a(0) = A(0,1), a(1) = A(0,2),
   a(2) = A(1,2), and so on. */

#include "../utils/bitlen_seq_t.h"   /* provides unsigned bitlen_seq_t(seq_t) */

/* Remainder of the polynomial P(n) divided by P(k) over GF(2). */
static seq_t poly_rem(idx_t n, idx_t k)
{
    seq_t r = (seq_t)n;
    seq_t d = (seq_t)k;           /* divisor polynomial */
    if (d == 0) return r;         /* k > 0 in the sequence but guard anyway */

    unsigned deg_d = bitlen_seq_t(d);

    while (r != 0) {
        unsigned deg_r = bitlen_seq_t(r);
        if (deg_r < deg_d) break;             /* division finished */
        unsigned shift = deg_r - deg_d;
        r ^= (d << shift);                    /* cancel the leading term */
    }
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find diagonal number d such that T(d) = d*(d+1)/2 <= n < T(d+1). */
    idx_t d = 0;
    while (((unsigned __int128)(d + 1) * (d + 2) / 2) <= n) {
        d++;
    }

    idx_t offset = n - d * (d + 1) / 2;   /* position inside this diagonal */
    idx_t i = d - offset;                /* row index (>= 0)          */
    idx_t k = offset + 1;                /* column index (>= 1)       */

    return poly_rem(i, k);
}
