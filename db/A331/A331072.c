/*
 * A331072  a(n) = Sum_{k = 1..n} u_3(k), where u_3 = A034836.
 * https://oeis.org/A331072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} u_3(k), where u_3(k) is the number of
 *            representations of k as a * b * c with 1 ≤ a ≤ b ≤ c
 * Range:     u8 n <= 83, u16 n <= 7587, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cuberoot_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_cuberoot_u64.h"

/* Compute floor(sqrt(n)) for a 64‑bit unsigned integer. */
static inline uint64_t floor_sqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (1ULL << 32);          /* hi > sqrt(ULLONG_MAX) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;        /* upper mid */
        if ((__uint128_t)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Count (b,c) with b*c = m, a_min ≤ b ≤ sqrt(m) and b ≤ c.
   (b ≤ c is automatic because b ≤ sqrt(m)). */
static inline seq_t count_b_pairs(uint64_t m, uint64_t a_min)
{
    seq_t cnt = 0;
    uint64_t lim = floor_sqrt_u64(m);
    for (uint64_t b = a_min; b <= lim; ++b) {
        if (m % b == 0)
            ++cnt;
    }
    return cnt;
}

/* u_3(n): number of triples (a,b,c) with 1≤a≤b≤c and abc = n. */
static inline seq_t u3_value(uint64_t n)
{
    if (n == 0)
        return 0;
    seq_t cnt = 0;
    uint64_t cuberoot = floor_cuberoot_u64(n);
    for (uint64_t a = 1; a <= cuberoot; ++a) {
        if (n % a == 0) {
            uint64_t m = n / a;
            cnt += count_b_pairs(m, a);
        }
    }
    return cnt;
}

/* Public function requested by the OEIS entry. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += u3_value((uint64_t)k);
    }
    return sum;
}
