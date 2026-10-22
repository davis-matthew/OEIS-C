/*
 * A330240  Square array T(n,k): concatenate the absolute differences of the
 *          digits of n and k (the smaller one padded with leading zeros),
 *          read by antidiagonals, n, k >= 0.
 * https://oeis.org/A330240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = T(i,j) where (i,j) is the position of n in the
 *            antidiagonal traversal of the square array T. T for (i,j) is
 *            defined as the integer obtained by concatenating the absolute
 *            differences of the decimal digits of i and j, with the shorter
 *            number padded with leading zeros
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the helper that provides the digit count of a uint64_t. */
#include "../utils/digits.h"

#include <stddef.h>
#include <stdint.h>

/* Pre‑computed powers of ten up to 10^19 (fits in uint64_t). */
static const idx_t POW10[20] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL
};

/* Return the value of the sequence at index n. */
seq_t a(idx_t n) {
    /* 1. Find the diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) {
        d++;
    }
    /* 2. Position inside the diagonal */
    idx_t offset = n - d * (d + 1) / 2;
    idx_t i = offset;
    idx_t j = d - offset;

    /* 3. Compute T(i,j) */
    int maxd = digits(i);
    int dj = digits(j);
    if (dj > maxd) maxd = dj;

    seq_t result = 0;
    for (int pos = maxd - 1; pos >= 0; pos--) {
        idx_t di = (i / POW10[pos]) % 10U;
        idx_t djm = (j / POW10[pos]) % 10U;
        idx_t diff = (di > djm) ? (di - djm) : (djm - di);
        result = result * 10 + (seq_t)diff;
    }
    return result;
}
