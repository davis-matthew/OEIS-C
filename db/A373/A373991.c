/*
 * A373991  a(n) = 1 if A328768(n) is a multiple of 3, otherwise 0, where
 *          A328768 is the first primorial based variant of arithmetic
 *          derivative.
 * https://oeis.org/A373991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if A328768(n) is a multiple of 3, otherwise 0 * *
 *            A328768(n) is defined by: D(p) = Primorial(p)/p * and extended
 *            to all n by Leibniz rule: D(ab)=a*D(b)+b*D(a). * * The algorithm
 *            computes D(n) modulo 3 using only the primes 2 and 3 * (all
 *            other primes contribute a multiple of 3), and ther...
 * Range:     u8 n <= 9325, u16 n <= 9325, u32 n <= 9325, u64 n <= 9325,
 *            u128 n <= 9325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special cases: a(0) = a(1) = 1 */
    if (n < 2) {
        return (seq_t)1;
    }

    uint64_t num = (uint64_t)n;        /* 64‑bit copy of n for integer ops */
    uint64_t rem = num;                /* remaining part to factor */
    uint8_t n_mod3 = num % 3u;         /* n mod 3, needed repeatedly */
    uint8_t sum_mod3 = 0;              /* accumulator for D(n) mod 3 */

    /* Trial division up to sqrt(rem) */
    for (uint64_t p = 2; p <= rem / p; ++p) {
        if (rem % p != 0) {
            continue;
        }

        /* Count the exponent of p in n */
        uint8_t exp_mod3 = 0;          /* exponent mod 3 */
        uint64_t exp = 0;
        while (rem % p == 0) {
            rem /= p;
            ++exp;
            exp_mod3 = (exp_mod3 + 1) % 3;
        }

        /* D(p) mod 3: 1 for p=2, 2 for p=3, 0 otherwise */
        uint8_t Dp_mod3;
        if (p == 2) {
            Dp_mod3 = 1;
        } else if (p == 3) {
            Dp_mod3 = 2;
        } else {
            Dp_mod3 = 0;
        }

        if (Dp_mod3 != 0) {
            if (p == 3) {
                /* term = (n/3) * exp * Dp  (mod 3) */
                uint8_t t = ((num / 3u) % 3u) * (exp_mod3 % 3u) % 3u;
                sum_mod3 = (sum_mod3 + t * Dp_mod3) % 3u;
            } else {
                /* For p != 3, compute (n/p) mod 3 using multiplicative inverse */
                uint8_t inv = (p % 3u == 1u) ? 1u : 2u; /* inverse modulo 3 */
                uint8_t n_div_mod3 = (n_mod3 * inv) % 3u;
                uint8_t t = (n_div_mod3 * exp_mod3) % 3u;
                sum_mod3 = (sum_mod3 + t * Dp_mod3) % 3u;
            }
        }
    }

    /* Handle a remaining prime factor larger than sqrt(original n) */
    if (rem > 1) {
        uint64_t p = rem;
        uint8_t Dp_mod3;
        if (p == 2) {
            Dp_mod3 = 1;
        } else if (p == 3) {
            Dp_mod3 = 2;
        } else {
            Dp_mod3 = 0;
        }

        if (Dp_mod3 != 0) {
            /* exponent is 1, so exp_mod3 = 1 */
            if (p == 3) {
                uint8_t t = ((num / 3u) % 3u) * 1u % 3u;
                sum_mod3 = (sum_mod3 + t * Dp_mod3) % 3u;
            } else {
                uint8_t inv = (p % 3u == 1u) ? 1u : 2u;
                uint8_t n_div_mod3 = (n_mod3 * inv) % 3u;
                uint8_t t = (n_div_mod3 * 1u) % 3u;
                sum_mod3 = (sum_mod3 + t * Dp_mod3) % 3u;
            }
        }
    }

    return (sum_mod3 == 0u) ? (seq_t)1 : (seq_t)0;
}
