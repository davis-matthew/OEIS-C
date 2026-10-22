/*
 * A329570  a(n) is the least prime P such that log(P)/log(p) >=
 *          valuation(n,p) for all primes p.
 * https://oeis.org/A329570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime P such that P ≥ max_{p^e‖n} p^e
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 1 the value is defined to be 2.                              */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)2;                /* a(1) = 2 */

    uint64_t num = (uint64_t)n;
    uint64_t max_power = 1;
    uint64_t temp = num;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t pow = 1;
            while (temp % p == 0) {
                temp /= p;
                pow *= p;
            }
            if (pow > max_power)
                max_power = pow;
        }
    }
    if (temp > 1) {                    /* remaining prime factor */
        if (temp > max_power)
            max_power = temp;
    }

    uint64_t cand = max_power;
    while (!is_prime(cand))
        ++cand;

    return (seq_t)cand;
}
