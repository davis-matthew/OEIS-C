/*
 * A354057  Square array read by ascending antidiagonals: T(n,k) is the number
 *          of solutions to x^k == 1 (mod n).
 * https://oeis.org/A354057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes a(n) from OEIS A354057.
 * The term a(n) is the n‑th element of the square array
 * read by ascending antidiagonals.
 * For an antidiagonal index s (s ≥ 1) the entries are
 *   (row, col) = (s, 1), (s-1, 2), … , (1, s).
 * Thus a(n) = T(row, col), where
 *   T(p^e, k) = gcd((p−1)p^(e−1), k)   for odd primes p,
 *   T(2, k)   = 1,
 *   T(2^e, k) = 1            if k is odd,
 *               2⋅gcd(2^(e-2), k)  if k is even,
 *   T(1, k)   = 1,
 *   T(m, k)   = ∏ T(p_i^e_i, k) over all prime powers in the factorisation of m.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Integer square root of a 128‑bit nonnegative number, floor value */
static inline uint64_t isqrt_u128(__int128 x)
{
    if (x <= 0)
        return 0;
    uint64_t low = 0;
    uint64_t high = 1;
    while ((__int128)high * high <= x)
        high <<= 1;
    low = high >> 1;
    while (low < high) {
        uint64_t mid = low + (high - low + 1) / 2;
        if ((__int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Find the smallest s such that s*(s+1)/2 ≥ pos */
static inline uint64_t find_s(uint64_t pos)
{
    uint64_t low = 0, high = 1;
    while (( (__int128)high * (high + 1) / 2 ) < pos)
        high <<= 1;
    while (low + 1 < high) {
        uint64_t mid = low + (high - low) / 2;
        if (( (__int128)mid * (mid + 1) / 2 ) >= pos)
            high = mid;
        else
            low = mid;
    }
    return high;
}

/* Compute T(n,k) as described above */
static inline seq_t compute_T(uint64_t n, seq_t k)
{
    if (n <= 1)
        return 1;

    seq_t res = 1;

    /* Handle powers of 2 */
    uint64_t e2 = 0;
    while ((n & 1ULL) == 0) {
        ++e2;
        n >>= 1;
    }
    if (e2 > 0) {
        if (e2 == 1) {
            /* T(2,k) = 1 – nothing to multiply */
        } else {
            /* e2 ≥ 2 */
            if ((k & 1ULL) == 0) { /* k even */
                seq_t two_pow = 1;
                for (uint64_t i = 0; i < e2 - 2; ++i)
                    two_pow <<= 1;           /* 2^(e2-2) */
                seq_t g = gcd(two_pow, k);
                res *= (seq_t)2 * g;        /* 2 * gcd(...) */
            }
            /* k odd -> contribution 1, already implicit in res */
        }
    }

    /* Factor the remaining odd part */
    uint64_t m = n;
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            /* compute (p-1) * p^(e-1) */
            seq_t factor = (seq_t)(p - 1);
            seq_t pow_pe = 1;
            for (uint64_t i = 1; i < e; ++i)
                pow_pe *= (seq_t)p;
            factor *= pow_pe;
            seq_t g = gcd(factor, k);
            res *= g;
        }
    }
    if (m > 1) { /* m is prime */
        seq_t factor = (seq_t)(m - 1);
        seq_t g = gcd(factor, k);
        res *= g;
    }
    return res;
}

/* Main sequence entry function */
seq_t a(idx_t n)
{
    uint64_t pos = (uint64_t)n;            /* 1‑based position in the sequence */
    uint64_t s   = find_s(pos);            /* diagonal index */

    uint64_t prev = (uint64_t)(((__int128)s * (s - 1)) / 2); /* entries before diagonal s */
    uint64_t offset = pos - prev - 1;                      /* 0‑based offset in diagonal s */

    uint64_t row = s - offset;          /* row index (≥1) */
    uint64_t col = offset + 1;          /* column index (≥1) */

    return compute_T(row, (seq_t)col);
}
