/*
 * A349147  Triangle T(n,m) read by rows: the sum of runs of all sequences
 *          arranging n objects of one type and m objects of another type.
 * https://oeis.org/A349147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r+m, r) * (2*r*m + r + m) / (r+m), where r =
 *            floor((sqrt(8*n+1)-1)/2), m = n - r*(r+1)/2
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }

    /* sum of first r natural numbers = r*(r+1)/2 */
    unsigned __int128 sum_before = (unsigned __int128)r * (r + 1) / 2;
    idx_t m = n - (idx_t)sum_before;          /* 0 <= m <= r */

    /* binomial(r+m, r) */
    seq_t bin = binomial(r + m, r);

    /* numerator and denominator of the fraction */
    seq_t num = (seq_t)2 * (seq_t)r * (seq_t)m + (seq_t)r + (seq_t)m;
    seq_t denom = (seq_t)(r + m);

    /* cancel common factors with binomial and denominator first */
    seq_t g1 = gcd(num, denom);
    if (g1 > 1)
        num /= g1, denom /= g1;

    seq_t g2 = gcd(bin, denom);
    if (g2 > 1)
        bin /= g2, denom /= g2;

    /* denom should now be 1 */
    return bin * num;
}
