/*
 * A372345  Square array A(n, k), n, k >= 0, read by upwards antidiagonals;
 *          for any n, k >= 0 with respective binary expansions Sum_{i >= 0}
 *          b_i * 2^i and Sum_{i >= 0} c_i * 2^i, A(n, k) = Sum_{i >= 0}
 *          (b_{i+1} * c_i + b_i * c_{i+1}) * 3^i.
 * https://oeis.org/A372345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = A(i,j) where A(i,j) = Σ_{k≥0} (b_{k+1}·c_k +
 *            b_k·c_{k+1})·3^k (i,j) is obtained from n by reading the array
 *            A(.,.) along upward antidiagonals: let m be the smallest integer
 *            with n < (m+1)(m+2)/2. Let offset = n – m(m+1)/2. Then i = m -
 *            offset, j = offset
 * Range:     u8 n <= 6252, u16 n <= 8146, u32 n <= 8146, u64 n <= 8146,
 *            u128 n <= 8146, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Computes the n-th term of OEIS sequence A372345. */
seq_t a(idx_t n)
{
    /* Find the diagonal index m and the offset within that diagonal. */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n)
        ++m;

    idx_t offset = n - m * (m + 1) / 2;
    idx_t i = m - offset;
    idx_t j = offset;

    /* Compute A(i,j) via the binary‑digit formula. */
    seq_t result = 0;
    seq_t pow3   = 1;          /* 3^k */

    idx_t bi = i;
    idx_t bj = j;

    while (bi != 0 || bj != 0)
    {
        unsigned int b_curr = bi & 1u;          /* b_k   */
        unsigned int c_curr = bj & 1u;          /* c_k   */
        unsigned int b_next = (bi >> 1) & 1u;   /* b_{k+1} */
        unsigned int c_next = (bj >> 1) & 1u;   /* c_{k+1} */

        unsigned int term = b_next * c_curr + b_curr * c_next;
        result += (seq_t)term * pow3;

        pow3 *= 3;
        bi >>= 1;
        bj >>= 1;
    }

    return result;
}
