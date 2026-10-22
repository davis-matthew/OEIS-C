/*
 * A366146  The sum of divisors of the largest divisor of n that is a cubefull
 *          number (A036966).
 * https://oeis.org/A366146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma(d) where d is the largest divisor of n that is
 *            cubefull
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_seq_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Here sigma is the sum of divisors, and d is the product of p^e for all
   primes p with exponent e in n that satisfy e ≥ 3.) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_seq_seq.h"

seq_t a(idx_t n) {
    uint64_t tmp = (uint64_t)n;          /* work in uint64_t for factorisation */
    seq_t result = 1;                    /* multiplicative identity */

    /* trial division, updating tmp as we go */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p != 0)
            continue;
        idx_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        if (e >= 3) {
            /* sigma(p^e) = (p^(e+1) - 1) / (p - 1) */
            seq_t pow = pow_seq_seq((seq_t)p, e + 1);
            seq_t sig = (pow - 1) / ((seq_t)p - 1);
            result *= sig;
        }
    }

    /* Any remaining prime factor has exponent 1, which is < 3,
       so it does not contribute to the cubefull divisor. */

    return result;
}
