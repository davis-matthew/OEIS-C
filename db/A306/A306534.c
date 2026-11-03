/*
 * A306534  Square array A(n,k), n >= 0, k >= 1, read by antidiagonals: A(n,k)
 *          = Sum_{j=0..n} floor(n/k^j).
 * https://oeis.org/A306534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t square_array_value(idx_t m_row, idx_t k_col)
{
    if (k_col == 1) {
        /* k = 1: A(m,k) = (m+1)*m */
        return ((seq_t)m_row * ((seq_t)m_row + 1));
    }

    seq_t sum = 0;
    seq_t power = 1;
    seq_t mm = (seq_t)m_row;

    while (power <= mm) {
        sum += mm / power;
        power *= (seq_t)k_col;
    }

    return sum;
}
/* Sequence A306534
   a(n) is the n-th element of the square array A(m,k) read by antidiagonals,
   where A(m,k) = Σ_{j=0..m} floor(m / k^j)  (k ≥ 1, m ≥ 0).
   The mapping from a linear index n to the array position (m,k) is:
     let s be the smallest integer with s*(s+1)/2 > n;
     let prev = (s-1)*s/2;
     let m = n - prev;        // 0 ≤ m < s
     let k = s - m;           // k ≥ 1
*/

seq_t a(idx_t n)
{
    /* find the diagonal s such that s*(s+1)/2 > n */
    idx_t s = 1;
    while ((unsigned __int128)s * (s + 1) / 2 <= n)
        ++s;

    /* previous total number of terms before diagonal s */
    idx_t prev = (s - 1) * s / 2;

    /* position inside diagonal s */
    idx_t m_row = n - prev;
    idx_t k_col = s - m_row;

    return square_array_value(m_row, k_col);
}
