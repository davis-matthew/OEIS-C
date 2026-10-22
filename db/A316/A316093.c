/*
 * A316093  Non-isomorphic colorings of the cube under rotations, using at
 *          most N colors on the faces and M colors on the vertices. Square
 *          array H(N,M) with N,M > 0 read by antidiagonals.
 * https://oeis.org/A316093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   antidiag_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A316093: 
 * H(N,M) = (N⁶M⁸ + 6N³M² + 3N⁴M⁴ + 8N²M⁴ + 6N³M⁴) / 24
 * a(n) = H(i,nj) where (i,j) are the antidiagonal (1‑based) coordinates of n. */
#include <stdint.h>
#include "../utils/antidiag_s.h"

seq_t a(idx_t n)
{
    /* find antidiagonal sum */
    idx_t s = antidiag_s(n);          /* minimal s with (s-1)*s/2 >= n */
    idx_t prev_total = (s - 2) * (s - 1) / 2;
    idx_t offset = n - prev_total - 1;    /* 0‑based within diagonal */
    idx_t i = s - offset - 1;             /* N */
    idx_t j = s - i;                      /* M */

    /* compute H(i,j) using 128‑bit intermediates to avoid overflow */
    __int128 ii = i;
    __int128 jj = j;

    __int128 term1 = ii * ii * ii * ii * ii * ii * jj * jj * jj * jj * jj * jj * jj * jj; /* N⁶M⁸ */
    __int128 term2 = (__int128)6 * ii * ii * ii * jj * jj;                                 /* 6 N³ M² */
    __int128 term3 = (__int128)3 * ii * ii * ii * ii * jj * jj * jj * jj;                /* 3 N⁴ M⁴ */
    __int128 term4 = (__int128)8 * ii * ii * jj * jj * jj * jj;                         /* 8 N² M⁴ */
    __int128 term5 = (__int128)6 * ii * ii * ii * jj * jj * jj * jj;                    /* 6 N³ M⁴ */

    __int128 sum = term1 + term2 + term3 + term4 + term5;
    __int128 result = sum / 24;

    return (seq_t)result;
}
