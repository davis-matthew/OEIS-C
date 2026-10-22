/*
 * A376180  Triangle read by rows (blocks). Each row consists of a permutation
 *          of the numbers of its constituents. The length of row number n is
 *          the n-th pentagonal number n(3n-1)/2 = A000326(n); see Comments.
 * https://oeis.org/A376180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   * a(n) is the n‑th entry of the triangle read by rows where *
 *            row r contains the consecutive integers from * S(r) + 1 to
 *            S(r+1) * (S(r) = (r‑1)²·r/2), permuted inside the block. * For
 *            position k (1‑based) in row r let * m = r(3r‑1)/2 // length of
 *            the row * c = (m+2)/2 // ceil((m+1)/2) * Then * if...
 * Range:     u8 n <= 222, u16 n <= 9126, u32 n <= 9126, u64 n <= 9126,
 *            u128 n <= 9126, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  Function to compute A376180  */
seq_t a(idx_t n)
{
    /*  Find the row r such that S(r) < n ≤ S(r+1).  */
    idx_t r = 1;
    /* use 128‑bit for the safety of the comparison */
    while ( ((unsigned __int128)r * r * (r + 1)) / 2 < n )
        ++r;

    unsigned __int128 r128 = r;
    unsigned __int128 offset = (r128 - 1) * (r128 - 1) * r128 / 2;   /* S(r) */
    unsigned __int128 k = n - offset;                                 /* 1‑based in row */

    unsigned __int128 m  = r128 * (3 * r128 - 1) / 2;                  /* length of row */
    unsigned __int128 c  = (m + 2) / 2;                                /* ceil((m+1)/2) */

    unsigned __int128 p;
    int k_odd = k & 1;
    int m_odd = m & 1;

    if (k_odd) {                 /* k is odd */
        if (m_odd)               /* m is odd  */
            p = (m - k + 2) / 2;
        else                      /* m is even */
            p = (k + m + 1) / 2;
    } else {                     /* k is even */
        if (m_odd)               /* m is odd  */
            p = c + k / 2;
        else                      /* m is even */
            p = c - k / 2;
    }

    unsigned __int128 result = offset + p;
    return (seq_t)result;
}
