/*
 * A363436  Array read by ascending antidiagonals: A(n, k) = k*n^2, with k >=
 *          0.
 * https://oeis.org/A363436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = j * i^2 where i = s - j, j = n - s(s+1)/2, s = floor(
 *            (sqrt(1+8n) - 1) / 2 ) (s is the diagonal index) The sequence is
 *            obtained by reading the array A(i,j)=j*i^2 along ascending
 *            antidiagonals (sum i+j)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_uint128(unsigned __int128 v) {
    idx_t low = 0, high = (idx_t)-1; /* sqrt fits in 64 bits */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 m2 = (unsigned __int128)mid * mid;
        if (m2 == v)
            return mid;
        if (m2 < v) {
            if (high == mid) { /* Prevent infinite loop when mid == high */
                low = mid + 1;
                break;
            }
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n) {
    /* Compute s = floor( (sqrt(1+8n) - 1) / 2 ) */
    unsigned __int128 rhs = (unsigned __int128)1 + (unsigned __int128)8 * n;
    idx_t sqrt_val = isqrt_uint128(rhs);
    idx_t s = (sqrt_val - 1) >> 1;  /* (sqrt_val - 1) / 2 */

    /* start index of diagonal s */
    unsigned __int128 start128 = (unsigned __int128)s * (s + 1) / 2;
    idx_t start = (idx_t)start128;

    /* j = n - start, i = s - j */
    idx_t j = n - start;
    idx_t i = s - j;

    /* value = j * i^2 */
    unsigned __int128 val = (unsigned __int128)j * i * i;
    return (seq_t)val;
}
