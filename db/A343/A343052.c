/*
 * A343052  Table read by ascending antidiagonals: T(k, n) is the minimum
 *          vertex sum in a perimeter-magic k-gon of order n.
 * https://oeis.org/A343052
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) for n≥3 is obtained by reading the table * T(k,n) = k*(1 +
 *            k + (n%2)*(1-(k%2)))/2 along ascending antidiagonals * (k+n
 *            constant), starting with (k=3,n=3)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 3; for other indices return 0. */
    if (n < 3U) return 0;

    /* 1‑based position of the term in the antidiagonal ordering. */
    uint64_t i = n - (uint64_t)2;

    /* Find the smallest m ≥ 1 such that m*(m+1)/2 ≥ i.
     * m is the number of elements in the diagonal containing the term. */
    uint64_t low = 0, high = 1;
    while (high * (high + 1) / 2 < i) high <<= 1;      /* growth phase */
    while (low + 1 < high) {                          /* binary search */
        uint64_t mid = (low + high) >> 1;
        if (mid * (mid + 1) / 2 >= i) high = mid;
        else low = mid;
    }
    uint64_t m = high;                               /* m ≥ 1 */

    /* s = sum k + n for the diagonal; k runs from s-3 down to 3. */
    uint64_t s = m + 5;

    /* Number of terms before this diagonal. */
    uint64_t total_before = (m - 1) * m / 2;

    /* 1‑based position inside the diagonal. */
    uint64_t pos = i - total_before;

    /* k decreases along the diagonal. */
    uint64_t k   = m + 3 - pos;
    /* Corresponding n. */
    uint64_t n2  = pos + 2;

    /* Compute T(k,n2) = k*(1 + k + (n2%2)*(1-(k%2)))/2. */
    uint64_t n_mod2 = n2 & 1U;
    uint64_t k_mod2 = k  & 1U;
    uint64_t extra  = n_mod2 * (1U - k_mod2);

    seq_t val = (seq_t)k * ((seq_t)1 + (seq_t)k + (seq_t)extra) / 2;
    return val;
}
