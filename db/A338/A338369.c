/*
 * A338369  Triangle read by rows: T(n,k) =
 *          (Sum_{i=0..n-k}(1+k*i)^3)/(Sum_{i=0..n-k} (1+k*i)) for 0 <= k <=
 *          n.
 * https://oeis.org/A338369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r is the row of the triangle with * 0 ≤ r
 *            and k = n - r(r+1)/2 (column in that row). T(r,k) is given by *
 *            T(r,k) = (k²·(r-k)² + k(k+2)(r-k) + 2) / 2. The sequence is *
 *            the triangle read by rows, starting with a(0)=T(0,0)=1
 * Range:     u8 n <= 38, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: given a non‑negative index n find the row r such that
 * r(r+1)/2 ≤ n < (r+1)(r+2)/2.  Uses only integer arithmetic. */
static idx_t row_from_n(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* Increase high until it is too big */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    /* Binary search in [low,high] */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Sequence entry */
seq_t a(idx_t n)
{
    /* Determine row r and column k */
    idx_t r = row_from_n(n);
    idx_t base = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - base;

    /* Compute T(r,k) using the explicit formula */
    seq_t rr = r;
    seq_t kk = k;
    seq_t nmk = rr - kk;

    seq_t term1 = kk * kk * nmk * nmk;                    /* k²·(r-k)² */
    term1 += kk * (kk + 2) * nmk;                         /* + k(k+2)(r-k) */
    term1 += 2;                                            /* + 2 */
    return term1 / 2;                                       /* divide by 2 */
}
