/*
 * A359200  Triangle read by rows: T(n, k) = A358125(n,k)*binomial(n-1, k), 0
 *          <= k <= n-1.
 * https://oeis.org/A359200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) with r = minimal integer satisfying r(r+1)/2 ≥ n
 *            and k = n – (r−1)r/2 – 1 (0 ≤ k ≤ r−1). T(r,k) = (2^r –
 *            2^(r−k−1) – 2^k) * binomial(r−1,k)
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

/* Find the row number r for a given 1‑based index n, using binary search. */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Return the nth term of A359200 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* safety guard – the sequence starts at n=1 */

    idx_t r = find_row(n);
    idx_t prev_total = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - prev_total - 1;   /* 0 ≤ k ≤ r−1 */

    seq_t term = pow2(r);
    term = term - pow2(r - k - 1);
    term = term - pow2(k);
    term = term * binomial(r - 1, k);

    return term;
}
