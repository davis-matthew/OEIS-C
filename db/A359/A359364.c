/*
 * A359364  Triangle read by rows. The Motzkin triangle, the coefficients of
 *          the Motzkin polynomials. M(n, k) = binomial(n, k) *
 *          CatalanNumber(k/2) if k is even, otherwise 0.
 * https://oeis.org/A359364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r,k be such that n = r(r+1)/2 + k, 0 ≤ k ≤ r. Then a(n) =
 *            M(r,k) where M(r,k) = binomial(r,k) * Catalan(k/2) if k is even,
 *            0 otherwise
 * Range:     u8 n <= 39, u16 n <= 127, u32 n <= 366, u64 n <= 1155,
 *            u128 n <= 4058, big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

/* integer square root of a 128‑bit value (floor) */
static inline uint64_t isqrt_u128(__uint128_t v)
{
    uint64_t low = 0, high = (uint64_t)-1;          /* 2^64‑1 */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= v) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Find the row r from the flat index n */
    __uint128_t v = (__uint128_t)8 * n + 1;
    uint64_t sqrt_v = isqrt_u128(v);
    uint64_t r = (sqrt_v - 1) / 2;

    __uint128_t tri = (__uint128_t)r * (r + 1) / 2;
    uint64_t k = n - (idx_t)tri;            /* column index in row r */

    /* Only even column indices contribute */
    if (k & 1ULL) {
        return (seq_t)0;
    }

    seq_t bin = binomial(r, k);
    seq_t cat = catalan(k / 2);

    return bin * cat;
}
