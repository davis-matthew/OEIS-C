/*
 * A384812  If n = Product prime(i)^e(i) then a(n) = Sum prime(e(i)).
 * https://oeis.org/A384812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over prime exponents e of prime(e) where n = ∏
 *            prime_i ^ e_i, with a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/smallest_prime_factor.h"

/* 64 primes – enough for exponents that appear in uint64_t factorizations. */
static const uint32_t _prime_of_exp[64] = {
    2,   3,   5,   7,   11,  13,  17,  19,  23,  29,
    31,  37,  41,  43,  47,  53,  59,  61,  67,  71,
    73,  79,  83,  89,  97,  101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
    233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
    283, 293, 307, 311
};

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1u)
        return (seq_t)0;

    uint64_t cur = n;          /* work with native 64‑bit integers   */
    seq_t     sum = (seq_t)0;   /* running total                    */

    while (cur > 1u) {
        uint64_t p = smallest_prime_factor(cur);  /* smallest prime factor   */
        uint64_t exp = 0;
        while (cur % p == 0u) {
            cur /= p;
            ++exp;
        }
        /* exp >= 1; map e -> prime(e) via pre‑computed table */
        sum += (seq_t)_prime_of_exp[(size_t)(exp - 1u)];
    }
    return sum;
}
