/*
 * A275257  Array read by upwards antidiagonals: LegendrePhi phi(x,n), x,n
 *          >=1.
 * https://oeis.org/A275257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = LegendrePhi(x, y), where (x, y) is the nth element of the
 *            array φ(x, y) (x, y ≥ 1) read by upward antidiagonals. The nth
 *            element lies on the diagonal s such that (s‑1)*s/2 < n ≤
 *            s*(s+1)/2, with x = s - (n - (s‑1)*s/2) + 1 and y = n -
 *            (s‑1)*s/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9828 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>

/* Legendre's totient sum φ(x, y):
   count of 1 ≤ k ≤ x coprime to y. */
static inline seq_t legendre_phi(idx_t x, idx_t y)
{
    if (y == 1) return (seq_t)x;

    /* collect distinct prime factors of y */
    idx_t pf[64];
    size_t pc = 0;
    idx_t rem = y;
    for (idx_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            pf[pc++] = p;
            while (rem % p == 0) rem /= p;
        }
    }
    if (rem > 1) pf[pc++] = rem;

    seq_t result = (seq_t)x;

    /* inclusion–exclusion over subsets of prime factors */
    for (size_t mask = 1; mask < (1ULL << pc); ++mask) {
        idx_t mult = 1;
        int bits = 0;
        for (size_t i = 0; i < pc; ++i) {
            if (mask & (1ULL << i)) {
                mult *= pf[i];
                ++bits;
            }
        }
        idx_t cnt = x / mult;
        if (bits & 1) result -= (seq_t)cnt;
        else          result += (seq_t)cnt;
    }
    return result;
}

/* Find minimal s for which s*(s+1)/2 ≥ n. */
static inline idx_t diag_of(idx_t n)
{
    idx_t low = 1, high = 1;
    while ((__uint128_t)high * (high + 1) / 2 < n) {
        high <<= 1;
        if (high == 0) {          /* overflow, clamp to maximum */
            high = UINT64_MAX;
            break;
        }
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__uint128_t)mid * (mid + 1) / 2 >= n) high = mid;
        else                                      low = mid + 1;
    }
    return low;
}

/* SEQUENCE FUNCTION */
seq_t a(idx_t n)
{
    /* locate diagonal and coordinates (x, y) */
    idx_t s = diag_of(n);
    __uint128_t before = ((__uint128_t)(s - 1) * s) / 2;
    idx_t offset = n - (idx_t)before;      /* 1‑based position in diagonal */
    idx_t x = s - offset + 1;              /* row index */
    idx_t y = offset;                      /* column index */

    return legendre_phi(x, y);
}
