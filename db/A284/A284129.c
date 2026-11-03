/*
 * A284129  Hosoya triangle Jacobsthal Lucas type.
 * https://oeis.org/A284129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = A014551(k) * A014551(n – k + 1), where n > 0 and 0 < k
 *            <= n. The sequence a(n) lists the triangle T(n,k) row‑by‑row,
 *            with offset 1. a(1) = T(1,1) = 1, a(2) = T(2,1) = 5, a(3) =
 *            T(2,2) = 5,
 * Range:     u8 n <= 22, u16 n <= 106, u32 n <= 466, u64 n <= 1954,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 73 terms through n = 4543 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute A014551 (Jacobs–Lucas numbers) for index m (m >= 0). */
static seq_t j_seq(idx_t m)
{
    if (m == 0) return (seq_t)2;
    if (m == 1) return (seq_t)1;

    idx_t exp = m - 1;          /* exponent for matrix M^(m-1)      */
    /* Matrix M = [[1, 2], [1, 0]] */
    seq_t M00 = 1, M01 = 2, M10 = 1, M11 = 0;
    /* Result matrix starts as identity */
    seq_t R00 = 1, R01 = 0, R10 = 0, R11 = 1;

    while (exp > 0) {
        if (exp & 1) {
            seq_t t00 = R00 * M00 + R01 * M10;
            seq_t t01 = R00 * M01 + R01 * M11;
            seq_t t10 = R10 * M00 + R11 * M10;
            seq_t t11 = R10 * M01 + R11 * M11;
            R00 = t00; R01 = t01; R10 = t10; R11 = t11;
        }
        seq_t t00 = M00 * M00 + M01 * M10;
        seq_t t01 = M00 * M01 + M01 * M11;
        seq_t t10 = M10 * M00 + M11 * M10;
        seq_t t11 = M10 * M01 + M11 * M11;
        M00 = t00; M01 = t01; M10 = t10; M11 = t11;
        exp >>= 1;
    }
    /* Multiply the result matrix by the vector [a1, a0] = [1, 2]  */
    seq_t value = R00 * (seq_t)1 + R01 * (seq_t)2;
    return value;
}

/* Find the smallest r such that r*(r+1)/2 >= n (1‑based). */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = ((idx_t)1) << 33;   /* upper bound > sqrt(2^64) */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Locate the row r that contains the n‑th element (row‑by‑row). */
    idx_t r = find_row(n);

    /* Number of elements before row r. */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t prev = (idx_t)prev_tri;   /* fits in idx_t for all n <= UINT64_MAX */

    /* Zero‑based position inside the row, then convert to 1‑based column k. */
    idx_t k = n - prev;            /* 1 <= k <= r */

    /* Compute the two Jacobs‑Lucas numbers: A014551(k) and A014551(r-k+1). */
    seq_t first  = j_seq(k);            /* k >= 1 */
    seq_t second = j_seq(r - k + 1);    /* r-k+1 >= 1 */

    return first * second;
}
