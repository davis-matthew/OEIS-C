/*
 * A323553  Irregular table read by rows: T(n,k) = (2*k+1)^(-1/3) mod 2^n, 0
 *          <= k <= 2^(n-1) - 1.
 * https://oeis.org/A323553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r, k), where r = floor_log2_uint64(n) + 1 (row number)
 *            k = n - 2^(r-1) (column index) T(r, k) is the unique odd integer
 *            x mod 2^r such that x^3 * (2*k + 1) ≡ 1 (mod 2^r). The routine
 *            computes the cube‑root of the inverse of (2*k+1) modulo 2^r
 *            using Hensel lifting
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"

static inline seq_t compute_T(idx_t r, idx_t k)
{
    uint64_t y = 2 * k + 1ULL;        /* odd factor */
    uint64_t x = 1ULL;                /* current root modulo 2^m */

    for (uint64_t m = 1; m < r; ++m) {
        /* compute f = x^3 * y - 1   (we only need bit m of f) */
        __uint128_t prod = (__uint128_t)x * x * x * y;
        uint64_t f = (uint64_t)prod - 1ULL;
        uint64_t q = (f >> m) & 1ULL;   /* correction bit for this level */
        x += q << m;
    }
    return (seq_t)x;
}

seq_t a(idx_t n)
{
    /* Find the row r such that 2^(r-1) <= n <= 2^r - 1 */
    unsigned r = floor_log2_uint64(n) + 1;

    uint64_t start = 1ULL << (r - 1);        /* first index of row r */
    uint64_t k = n - start;                 /* column index (0‑based) */

    return compute_T(r, k);
}
