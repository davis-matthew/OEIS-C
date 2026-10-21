/*
 * A286100  Square array A(n,k): If n = k, then A(n,k) = n, otherwise 0, read
 *          by antidiagonals as A(1,1), A(1,2), A(2,1), A(1,3), A(2,2),
 *          A(3,1), etc.
 * https://oeis.org/A286100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k if n = 2k(k-1)+1 else 0
 * Range:     u8 n <= 9687, u16 n <= 9687, u32 n <= 9687, u64 n <= 9687,
 *            u128 n <= 9687, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute s = 2*n - 1 using 128-bit intermediate to avoid overflow. */
    unsigned __int128 s = ((unsigned __int128)2 * n) - 1;

    /* Binary search for floor(sqrt(s)).  The result fits into 64 bits. */
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64);
    unsigned __int128 r = 0;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 mid2 = mid * mid;
        if (mid2 <= s) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* Check if s is a perfect square. */
    if (r * r != s)
        return (seq_t)0;

    /* r = sqrt(2*n - 1), which is odd.  k = (1 + r)/2. */
    unsigned long long k = ((unsigned long long)r + 1ULL) >> 1;
    return (seq_t)k;
}
