/*
 * A391926  Square array read by descending antidiagonals: find the k-th
 *          natural number i that satisfies i*n = A048720(i,m) for some m,
 *          then A(n, k) = m. Here A048720 is carryless base-2 multiplication.
 * https://oeis.org/A391926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 764, u16 n <= 9744, u32 n <= 9744, u64 n <= 9744,
 *            u128 n <= 9744, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A391926 - Square array read by descending antidiagonals
 *
 *   For a given row `n` (1‑based) the array entries A(n,k) are defined as
 *
 *      Let i be the k‑th natural number for which there exists an m such
 *      that i*n = A048720(i,m)  (carryless base‑2 multiplication).
 *      Then A(n,k) = m.
 *
 *   The sequence a(n) is obtained by reading the 2‑D array A in
 *   descending anti‑diagonals:
 *     A(1,1),
 *     A(1,2), A(2,1),
 *     A(1,3), A(2,2), A(3,1), …
 *
 *   Implementation notes:
 *     * Only integer arithmetic is used; no floating point or <math.h>.
 *     * `bitlen_seq_t` is supplied by the build; it returns the
 *       position of the highest set bit (0 for zero).
 *     * The carryless product is tested by polynomial division over GF(2);
 *       if the division remainder is zero, the quotient is the desired m.
 *     * Each row is searched linearly; for the limits used in the tests
 *       this is fast enough (g ≤ 200 for n ≤ 10 000).
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Carryless (XOR‑based) binary multiplication of a * b. */
static inline seq_t carryless_mul(seq_t a, seq_t b)
{
    seq_t res = 0;
    while (b) {
        if (b & 1) res ^= a;
        a <<= 1;
        b >>= 1;
    }
    return res;
}

/* Polynomial division over GF(2).  dividend = divisor * *quotient  */
static inline bool poly_div_exact(seq_t dividend, seq_t divisor, seq_t *quotient)
{
    if (divisor == 0) return false;
    seq_t q = 0;
    seq_t a = dividend;
    unsigned deg_div = bitlen_seq_t(divisor) - 1;
    while (a != 0 && bitlen_seq_t(a) >= bitlen_seq_t(divisor)) {
        unsigned deg_a  = bitlen_seq_t(a) - 1;
        unsigned shift  = deg_a - deg_div;
        q |= ((seq_t)1 << shift);
        a ^= (divisor << shift);
    }
    *quotient = q;
    return a == 0;
}

/* Return the entry A(row_n, col_k).  row_n, col_k are 1‑based. */
static seq_t row_col_value(idx_t row_n, idx_t col_k)
{
    idx_t found = 0;
    for (seq_t i = 1; ; ++i) {
        /* normal integer product: i * row_n  */
        unsigned __int128 prod128 = (unsigned __int128)i * (unsigned __int128)row_n;
        seq_t prod = (seq_t)prod128;           /* cast to seq_t */

        seq_t m;
        if (!poly_div_exact(prod, (seq_t)i, &m))
            continue;                         /* i does not satisfy */

        if (++found == col_k)
            return m;                         /* found the k‑th */
    }
}

/* Compute a(n): nth element in the antidiagonal reading. */
seq_t a(idx_t n)
{
    /* Find the diagonal g that contains element n.
       The g-th diagonal has g elements, and the total up to
       diagonal g is g*(g+1)/2.  We compute g from n.   */
    idx_t g = 1;
    /* exponential search to get an upper bound */
    while ((unsigned __int128)g * (g + 1u) / 2u < n)
        g <<= 1;

    /* binary search within [1, g] */
    idx_t low  = 0;
    idx_t high = g;
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1u) / 2u;
        if (val < n)
            low = mid;
        else
            high = mid;
    }
    g = high;

    /* previous cumulative count (up to diagonal g-1) */
    unsigned __int128 prev = (unsigned __int128)(g - 1u) * g / 2u;
    idx_t offset = (idx_t)(n - prev - 1u);   /* 0‑based within diagonal */

    idx_t row = offset + 1u;                 /* 1‑based row index   */
    idx_t col = g - offset;                  /* 1‑based column index */

    return row_col_value(row, col);
}
