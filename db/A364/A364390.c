/*
 * A364390  Triangle T(n, k) based on A176040 which read by rows yields a
 *          permutation of the positive integers.
 * https://oeis.org/A364390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   T(n, k) = n*(n+1)/2 + (n-1)*(n mod 2) - 2*k + 3 - (k mod 2) a(n)
 *            is obtained by reading T(n, k) row‑wise (row major order) and
 *            a(1) = 1
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* row r satisfies  r*(r+1)/2  ≥  n  and  (r-1)*r/2  <  n   */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    idx_t prev = r * (r - 1) / 2;        /* number of terms before row r */
    idx_t k = n - prev;                  /* column in the r‑th row */

    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;

    seq_t val = sr * (sr + 1) / 2
              + ( (seq_t)(r - 1) ) * ( (seq_t)(r % 2) )
              - 2 * sk
              + 3
              - ((seq_t)(k % 2));
    return val;
}
