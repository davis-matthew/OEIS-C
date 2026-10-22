/*
 * A332502  Rectangular array read by antidiagonals: T(n,k) = floor(n + k*r),
 *          where r = golden ratio = (1+sqrt(5))/2.
 * https://oeis.org/A332502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n-th term of the antidiagonal reading of T(i,j) =
 *            floor(i + j·φ) with φ = (1+√5)/2
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned __int128 value.
   Returns floor(sqrt(v)). */
static inline uint64_t isqrt128(unsigned __int128 v) {
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64) - 1;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq == v)
            return (uint64_t)mid;
        if (sq < v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return (uint64_t)high;
}

/* Return the n-th term of A332502. */
seq_t a(idx_t n) {
    idx_t m = n;

    /* Find the largest s such that s*(s+1)/2 <= m. */
    idx_t low = 0;
    idx_t high = m + 1;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= m)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t s = low;

    /* Offset within diagonal s. */
    idx_t tri_s = low * (low + 1) / 2;
    idx_t offset = m - tri_s;

    idx_t i = offset;          /* row (vertical index) */
    idx_t j = s - offset;      /* column (horizontal index) */

    /* Compute floor(j·φ) using floor(j·√5 + j)/2. */
    unsigned __int128 prod = (unsigned __int128)5 * j * j;
    uint64_t sqrt_term = isqrt128(prod);          /* floor(√(5·j²)) */
    uint64_t floor_j_times_phi = (j + sqrt_term) / 2;

    /* Final value: i + floor(j·φ). */
    uint64_t result = (uint64_t)i + floor_j_times_phi;

    return (seq_t)result;
}
