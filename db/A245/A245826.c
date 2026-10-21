/*
 * A245826  Triangle read by rows: T(m,n) is the Szeged index of the grid
 *          graph P_m X P_n (1 <= n <= m).
 * https://oeis.org/A245826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k) where T(m,n) = mn(2m²n² - m² - n²)/6
 * Range:     u8 n <= 8, u16 n <= 35, u32 n <= 1223, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a is read row‑by‑row: row m contains T(m,1), T(m,2), … , T(m,m). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row m such that m(m+1)/2 >= n. */
    idx_t low = 1, high = n, mid, m = n;
    while (low <= high) {
        mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) {
            m = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    /* Offset of the first element of row m. */
    unsigned __int128 offs = (unsigned __int128)(m - 1) * m / 2;
    idx_t offset = (idx_t)offs;

    /* Position k within the row (1 ≤ k ≤ m). */
    idx_t k = n - offset;

    /* Compute T(m,k) using 128–bit intermediates. */
    unsigned __int128 a_ = m;
    unsigned __int128 b_ = k;
    unsigned __int128 mm = a_ * a_;
    unsigned __int128 nn = b_ * b_;
    unsigned __int128 c  = 2 * mm * nn - mm - nn;
    unsigned __int128 prod = a_ * b_ * c;
    prod /= 6;

    return (seq_t)prod;
}
