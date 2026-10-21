/*
 * A294873  a(n) = Product_{d|n, d>1, d = x^(2k-1) for some maximal k >= 1}
 *          prime(k).
 * https://oeis.org/A294873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{d|n, d>1, d = x^(2k-1) (maximal k ≥ 1)} prime(k)
 * Range:     u8 n <= 59, u16 n <= 239, u32 n <= 1259, u64 n <= 9179,
 *            u128 n <= 9179, big any n
 * Helpers:   factor_u64, gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* prime(k) = the k‑th prime (k ≥ 1).                                            */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd_u64.h"

/* First 32 primes, enough for k ≤ 32 (necessary for n < 2^63). */
static const uint32_t prime_of_k[32] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131
};

/* Euclidean GCD for uint64_t. */

/* Recursive generation of all divisors d>1 of n.  cur_gcd keeps the
 * greatest common divisor of the exponents of the currently chosen
 * prime factors.  If cur_gcd is odd, d is not a perfect even power
 * and contributes prime((cur_gcd+1)/2). */
static void rec_divisors(const uint64_t *pr, const uint64_t *ex,
                         size_t cnt, size_t idx,
                         uint64_t cur_gcd, bool chosen,
                         seq_t *prod)
{
    if (idx == cnt) {
        if (chosen && (cur_gcd & 1ULL)) {          /* odd cur_gcd   */
            uint64_t k = (cur_gcd + 1ULL) / 2ULL;   /* k ≥ 1          */
            if (k >= 1 && k <= 32) {
                *prod = (*prod) * (seq_t)prime_of_k[k - 1];
            }
        }
        return;
    }

    /* branch: skip this prime (exponent 0) */
    rec_divisors(pr, ex, cnt, idx + 1, cur_gcd, chosen, prod);

    /* branches: use exponent e = 1 .. ex[idx] */
    for (uint64_t e = 1; e <= ex[idx]; ++e) {
        uint64_t new_gcd = chosen ? gcd_u64(cur_gcd, e) : e;
        rec_divisors(pr, ex, cnt, idx + 1, new_gcd, true, prod);
    }
}

/* Main entry routine for the sequence. */
seq_t a(idx_t n)
{
    if (n <= 1) return 1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* factorize n */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = 1;
    rec_divisors(primes, exps, cnt, 0, 0, false, &prod);
    return prod;
}
