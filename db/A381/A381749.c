/*
 * A381749  Triangle read by rows: T(n,k), n >= k, is the maximum number of
 *          kings on a n X k chessboard so that no king attacks more than one
 *          other king.
 * https://oeis.org/A381749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) computed from triangle T(n,k): * • T(3*m,3) = 4*m * •
 *            T(6*m+3,6) = 14*m+8 * • otherwise = floor((n+1)(k+1)/3) - [
 *            (n+1)(k+1) mod 6 == 3 ] * and a(n) is the (n)th element of the
 *            triangle read by rows. * Indexing: a(1) = T(1,1)
 * Range:     u8 n <= 376, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* safety for n < 1, offset starts at 1 */

    /* Find the row r such that r*(r+1)/2 >= n */
    uint64_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    while (low < high) {
        uint64_t mid = (low + high) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    uint64_t r = low;                     /* row number */

    /* Compute column k (1‑based) */
    unsigned __int128 start128 = (unsigned __int128)r * (r - 1) / 2;
    uint64_t start = (uint64_t)start128;   /* first index of this row */
    uint64_t k = n - start;                /* column index */

    /* Special cases */
    if (k == 3 && r % 3 == 0) {
        return (seq_t)(4ull * (r / 3));
    }
    if (k == 6 && r % 6 == 3) {
        uint64_t m = (r - 3) / 6;
        return (seq_t)(14ull * m + 8);
    }

    /* Generic formula */
    seq_t n_plus = (seq_t)(r + 1);
    seq_t k_plus = (seq_t)(k + 1);
    seq_t prod   = n_plus * k_plus;
    seq_t val    = prod / 3;
    seq_t rem    = prod % 6;
    if (rem == 3) {
        val -= 1;
    }
    return val;
}
