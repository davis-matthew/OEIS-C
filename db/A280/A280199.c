/*
 * A280199  Numbers n such that a^(n-1) == 1 (mod n^2) has solutions with 1 <
 *          a < n^2-1.
 * https://oeis.org/A280199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 9917, u32 n <= 9917, u64 n <= 9917,
 *            u128 n <= 9917, big any n
 * Helpers:   gcd_uint64, has_odd_prime_with_gcd, is_power_of_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A280199: Numbers n such that a^(n-1) == 1 (mod n^2) has solutions with 1 < a < n^2-1.
   Implementation uses the characterization:
     – All odd n ≥ 5 that are not a power of 3.
     – All even n that have at least one odd prime factor p with gcd(n‑1, p‑1) > 1. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"
#include "../utils/has_odd_prime_with_gcd.h"
#include "../utils/is_power_of_three.h"

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
