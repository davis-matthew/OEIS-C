/*
 * A289226  Number of ways to select 5 disjoint point triples from an n X n X
 *          n triangular point grid, each point triple forming a 2 X 2 X 2
 *          triangle.
 * https://oeis.org/A289226
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^10 -10*n^9 -85*n^8 +1160*n^7 +1345*n^6 -49162*n^5 *
 *            +62145*n^4 +892140*n^3 -2198566*n^2 -5725008*n +18190440)/120
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 16, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 5) return 0;                      /* indices below the offset are undefined */

    /* Coefficients of the 10th‑degree polynomial (highest first) */
    static const int64_t coeffs[11] = {
        1LL,      -10LL,   -85LL,   1160LL,   1345LL,
       -49162LL,  62145LL,  892140LL,-2198566LL,-5725008LL,18190440LL
    };

    /* If seq_t fits in 64 bits, use 128‑bit intermediates to avoid overflow.
       Otherwise use seq_t for all arithmetic (assumed arbitrary‑precision). */
    if (sizeof(seq_t) <= sizeof(uint64_t))
    {
        /* Use signed __int128 for the Horner evaluation. */
        signed __int128 v = (signed __int128)coeffs[0];
        for (int i = 1; i < 11; ++i)
        {
            v = v * (signed __int128)n + (signed __int128)coeffs[i];
        }
        v /= 120;                              /* exact division */
        return (seq_t)v;                       /* cast to the return type */
    }
    else
    {
        seq_t v = (seq_t)coeffs[0];
        for (int i = 1; i < 11; ++i)
        {
            v = v * n + (seq_t)coeffs[i];
        }
        v /= 120;
        return v;
    }
}
