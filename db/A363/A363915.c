/*
 * A363915  Triangle read by rows. The indicator function for prime divisors,
 *          the Euclid matrix.
 * https://oeis.org/A363915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangle T, where T(i,j) = 1 if j
 *            is a prime divisor of i (i.e. j prime and j | i), otherwise
 *            T(i,j) = 0. The triangle is read row‑by‑row, so a(n) = T(i,j)
 *            for the unique i,j with n = i*(i+1)/2 + j, 0 ≤ j ≤ i
 * Range:     u8 n <= 9569, u16 n <= 9569, u32 n <= 9569, u64 n <= 9569,
 *            u128 n <= 9569, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* 128‑bit arithmetic is available in both C11 and C++17. */
static inline idx_t triangular(idx_t i) {
    return (idx_t)(((__uint128_t)i * (i + 1)) / 2);
}

/* Find the row index i such that triangular(i) <= n < triangular(i+1). */
static inline idx_t find_row(idx_t n) {
    idx_t low = 0, high = 1;

    /* Expand upper bound until triangular(high) > n. */
    while (triangular(high) <= n) {
        high <<= 1;
        if (high == 0) {          /* overflow, cap at max 32‑bit value */
            high = 0xFFFFFFFFu;
            break;
        }
    }

    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (triangular(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Locate the row (i) and column (j) corresponding to flat index n. */
    idx_t i = find_row(n);
    idx_t tri = triangular(i);
    idx_t j = n - tri;          /* 0 ≤ j ≤ i */

    /* Indicator rule: j must be prime and divide i. */
    if (j < 2)                /* 0 or 1 is never prime */
        return (seq_t)0;
    if (!is_prime(j))
        return (seq_t)0;
    return (seq_t)((i % j) == 0);
}
