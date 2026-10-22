/*
 * A331147  Triangle read by rows: T(n,k) (n>=k>=1) = floor((n/k)*floor(n/k)).
 * https://oeis.org/A331147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor((r/k) * floor(r/k)), where r and k are determined
 *            by unfolding the triangle with T(r,k)=floor((r/k)*floor(r/k))
 *            such that n = r*(r-1)/2 + k and 1 <= k <= r
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find r :   r*(r+1)/2 >= n > (r-1)*r/2   */
    idx_t low = 1;
    idx_t high = n;                /* r never exceeds n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* k is the position inside row r */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev;     /* 1 <= k <= r */

    /* Compute T(r,k) = floor( (r/k) * floor(r/k) ) */
    idx_t m = r / k;                               /* floor(r/k) */
    unsigned __int128 prod = (unsigned __int128)r * m; /* r * m   */
    seq_t result = (seq_t)(prod / k);               /* floor(prod/k) */

    return result;
}
