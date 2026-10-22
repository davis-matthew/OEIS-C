/*
 * A354555  Rectangular array read by antidiagonals. T(m,n) is the number of
 *          degree n monic polynomials in GF_2[x] such that each irreducible
 *          factor in the prime factorization has multiplicity no greater than
 *          m, m>=1, n>=0.
 * https://oeis.org/A354555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(m,n)=2^n for n<=m, else 2^n-2^(n-m). The sequence is the
 *            rectangular array T read by antidiagonals (m>=1, n>=0)
 * Range:     u8 n <= 53, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static idx_t diagonal_of(idx_t k)
{
    /* Find the diagonal number s such that s*(s-1)/2 <= k < s*(s+1)/2. */
    idx_t s = 1;
    while (s * (s - 1) / 2 <= k) {
        s++;
    }
    return s - 1;
}

seq_t a(idx_t n)
{
    /* Convert the flat index n into (m,n) coordinates. */
    idx_t s = diagonal_of(n);                /* diagonal (sum) */
    idx_t prev = s * (s - 1) / 2;            /* number of entries before this diagonal */
    idx_t offset = n - prev;                 /* position inside the diagonal (0-based) */
    idx_t m = s - offset;                    /* decreasing m */
    idx_t deg = offset;                      /* degree in GF_2[x] */

    /* Compute the value T(m,deg). */
    seq_t pow_deg = pow2(deg);                /* 2^deg */
    if (deg <= m) {
        return pow_deg;
    } else {
        return pow_deg - pow2(deg - m);       /* 2^deg - 2^(deg-m) */
    }
}
