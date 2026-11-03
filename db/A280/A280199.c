/*
 * A280199  Numbers n such that a^(n-1) == 1 (mod n^2) has solutions with 1 <
 *          a < n^2-1.
 * https://oeis.org/A280199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 17948, u32 n <= 17948, u64 n <= 17948,
 *            u128 n <= 17948, big any n
 * Validated: standard, pass, 81 terms through n = 9917 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A280199: Numbers n such that a^(n-1) == 1 (mod n^2) has solutions with 1 < a < n^2-1.
   Implementation uses the characterization:
     – All odd n ≥ 5 that are not a power of 3.
     – All even n that have at least one odd prime factor p with gcd(n‑1, p‑1) > 1. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

/* For even n, return true if some odd prime factor p of n satisfies
   gcd(n‑1, p‑1) > 1. */
static bool has_odd_prime_with_gcd(uint64_t n) {
    /* Strip all factors of 2 to obtain the odd part. */
    uint64_t odd = n;
    while ((odd & 1) == 0)
        odd >>= 1;

    if (odd <= 1)
        return false;   /* no odd prime factors */

    uint64_t m = odd;
    /* Trial division of the odd part. */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            if (gcd_u64(n - 1, p - 1) > 1)
                return true;
            while (m % p == 0)
                m /= p;
        }
    }
    /* Remaining prime factor larger than sqrt(odd). */
    if (m > 1 && gcd_u64(n - 1, m - 1) > 1)
        return true;

    return false;
}

/* Return true if n is a power of 3 (3^k, k ≥ 1). */
static bool is_power_of_three(uint64_t n) {
    if (n < 3) return false;
    while (n % 3 == 0)
        n /= 3;
    return n == 1;
}

/* Euclidean gcd for 64‑bit unsigned integers. */

/* Check whether n belongs to A280199. */
static bool in_sequence(uint64_t n) {
    if (n < 5)
        return false;

    if ((n & 1) == 1)          /* odd n */
        return !is_power_of_three(n);

    /* even n: need gcd condition with an odd prime factor */
    return has_odd_prime_with_gcd(n);
}

/* Sequence function requested by the build system. */
seq_t a(idx_t n) {
    idx_t idx = 1;             /* current index (a(1)=5) */
    uint64_t val = 5;          /* current candidate integer */

    while (idx < n) {
        ++val;
        if (in_sequence(val))
            ++idx;
    }
    return (seq_t)val;
}
