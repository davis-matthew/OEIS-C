/*
 * A358052  Triangular array read by rows. For T(n,k) where 1 <= k <= n, start
 *          with x = k and repeat the map x -> floor(n/x) + (n mod x) until an
 *          x occurs that has already appeared. The number of applications of
 *          the map is T(n,k).
 * https://oeis.org/A358052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where the triangular index r is found by solving
 *            r(r+1)/2 >= n > (r-1)r/2, and k = n - r(r-1)/2. T(r,k) is the
 *            number of times the map f(x) = floor(r / x) + (r % x) must be
 *            applied to x=k until a repeated value occurs
 * Range:     u8 n <= 9223, u16 n <= 9223, u32 n <= 9223, u64 n <= 9223,
 *            u128 n <= 9223, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* find row r such that r(r+1)/2 >= n > (r-1)r/2 */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if ((idx_t)tri >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;  /* column index */

    /* compute T(r,k) */
    uint8_t *seen = (uint8_t *)calloc((size_t)(r + 1), sizeof(uint8_t));
    if (!seen) return 0;  /* allocation failed; return 0 as fallback */
    idx_t x = k;
    idx_t steps = 0;
    while (!seen[x]) {
        seen[x] = 1;
        idx_t q = r / x;
        idx_t rem = r % x;
        x = q + rem;
        steps++;
    }
    free(seen);
    return (seq_t)steps;
}
