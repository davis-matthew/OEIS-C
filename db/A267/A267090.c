/*
 * A267090  Triangle read by rows: Fill an n X n square with 1's, except for
 *          0's on the two main diagonals. Then T(n,k) is decimal equivalent
 *          of the k-th row (0<=k<=n).
 * https://oeis.org/A267090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  A267090: triangle read by rows
 *
 *  For each non‑negative integer n we first determine the triangle of size s
 *  such that n lies between the triangular numbers T_{s-1} = s*(s-1)/2
 *  and T_{s}   = s*(s+1)/2 - 1.  The offset k of n in that block is
 *
 *        k = n - s*(s-1)/2          (0 <= k < s)
 *
 *  The row of the s×s matrix is described by binary digits: the main
 *  diagonal and the secondary diagonal are 0, all other positions are 1.
 *  We read the row from left to right as a binary number (most significant
 *  bit first) and return its decimal value.
 *
 *  The function works for any seq_t: 8,16,32,64 bit or arbitrary‑precision.
 *  The algorithm runs in O(s) time where s ≈ √(2n), which is well below
 *  the 2‑second limit for n up to 10 000.
 */

#include <stdint.h>
#include <math.h>

/* Compute the sequence value a(n) */
seq_t a(idx_t n)
{
    /* Determine s: the size of the triangle containing n */
    /* s satisfies: s*(s-1)/2 <= n < s*(s+1)/2 */
    idx_t s;

    /* Compute integer m = floor((sqrt(8*n+1)-1)/2) */
    /* Use double for speed, then correction for any rounding error */
    long double d = sqrt((long double)8 * n + 1);
    idx_t m = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct possible off‑by‑one due to floating point rounding */
    while ((unsigned __int128)m * (m + 1) / 2 > n) --m;
    while ((unsigned __int128)(m + 1) * (m + 2) / 2 <= n) ++m;

    s = m + 1;

    /* Row offset within the triangle */
    idx_t start = (unsigned __int128)s * (s - 1) / 2;
    idx_t k = n - start;                     /* 0 <= k < s */

    /* Build the binary row and convert to decimal */
    seq_t val = 0;
    for (idx_t j = 0; j < s; ++j)
    {
        seq_t bit = ((j == k) || (j == s - 1 - k)) ? 0 : 1;
        val = (val << 1) | bit;
    }

    return val;
}
