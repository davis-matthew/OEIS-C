/*
 * A333119  Triangle T read by rows: T(n, k) = (n - k)*(1 - (-1)^k + 2*k)/4,
 *          with 0 <= k < n.
 * https://oeis.org/A333119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k), where r = ceil((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r-1)/2 - 1, and T(r,k) = (r-k)*(1 - (-1)^k + 2*k)/4.
 *            Equivalently, for k even: T(r,k) = k*(r-k)/2, for k odd : T(r,k)
 *            = (k+1)*(r-k)/2
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for out‑of‑range calls */

    /* Find the row index r such that r*(r+1)/2 >= n. */
    idx_t low  = 1;
    idx_t high = n;          /* n is never 0 here */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        /* mid*(mid+1)/2 using 128‑bit arithmetic to avoid overflow */
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        if (prod >= (unsigned __int128)n * 2)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Number of entries preceding row r: pre = (r-1)*r/2 */
    unsigned __int128 prodpre = (unsigned __int128)(r - 1) * r;
    idx_t pre = (idx_t)(prodpre / 2);

    /* Position of n within row r: k = n - pre - 1 */
    idx_t k = n - pre - 1;

    /* Compute the triangle entry using the parity of k. */
    if ((k & 1) == 0) {           /* k even */
        return ((seq_t)k) * ((seq_t)(r - k)) / 2;
    } else {                       /* k odd */
        return ((seq_t)(k + 1)) * ((seq_t)(r - k)) / 2;
    }
}
