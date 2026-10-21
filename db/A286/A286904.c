/*
 * A286904  Positions of 0 in A286903; complement of A286905.
 * https://oeis.org/A286904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + (n mod 2))*r) + n, where r = (1+sqrt(2))/2 *
 *            Equivalently * a(2k) = floor(k*sqrt(2)) + 3k * a(2k+1) =
 *            floor((k+1)*sqrt(2)) + 3k + 2
 * Range:     u8 n <= 22, u16 n <= 395, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t int_sqrt(seq_t x)
/* Integer floor of sqrt(x) using binary search.  The function works for
 * any unsigned seq_t size because the intermediate
 * __uint128_t multiplication stays within bounds for the values
 * encountered in this sequence (n <= 10⁴ in the tests). */
{
    seq_t low = 0, high = (x >> 1) + 1, mid, result = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        __uint128_t mid2 = (__uint128_t)mid * (__uint128_t)mid;
        if (mid2 <= x) {
            result = mid;
            low = mid + 1;
        } else {
            high = (mid == 0) ? 0 : mid - 1;
        }
    }
    return result;
}

static seq_t floor_n_sqrt2(idx_t n)
/* Compute floor(n * sqrt(2)) by taking the square root of 2 * n * n.
 * This avoids floating‑point arithmetic and works for all supported
 * seq_t sizes. */
{
    __uint128_t mtmp = (__uint128_t)n * n * 2;
    seq_t m = (seq_t)mtmp;
    return int_sqrt(m);
}

seq_t a(idx_t n)
/* Main sequence function. */
{
    if (n == 0) return 0;                      /* undefined below offset */

    if ((n & 1) == 0) {                        /* n = 2k  */
        idx_t k = n >> 1;
        seq_t part = floor_n_sqrt2(k);
        return part + (seq_t)(3 * k);
    } else {                                   /* n = 2k+1  */
        idx_t k = (n - 1) >> 1;
        seq_t part = floor_n_sqrt2(k + 1);
        return part + (seq_t)(3 * k + 2);
    }
}
