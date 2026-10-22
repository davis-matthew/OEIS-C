/*
 * A346759  a(n) = Sum_{d|n} floor(d^2/4).
 * https://oeis.org/A346759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n} floor(d^2/4)
 * Range:     u8 n <= 27, u16 n <= 419, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* sum_floor_divisors -- compute a(n) by generating all divisors of n.
   The divisor generation uses an iterated multiplication scheme that
   avoids producing non‑divisors.
   Complexity:  O(number_of_divisors). */
static seq_t sum_floor_divisors(idx_t n)
{
    /* factor_u64 fills arrays with the prime factors and their exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    /* Stack array for divisors – 128 divisors is more than sufficient for
       n <= 10 000 (the maximum tested).  For the arbitrary‑precision build
       the type seq_t can hold all needed values. */
    seq_t divs[256];
    size_t sz = 1;                /* current number of divisors stored */
    divs[0] = 1;                  /* divisor 1 */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t exp = exps[i];
        uint64_t p_pow = 1;
        size_t base_sz = sz;      /* divisors before considering this prime */

        /* For each exponent f = 1 .. exp, multiply all existing divisors
           (those in divs[0..base_sz-1]) by p^f. */
        for (uint64_t f = 1; f <= exp; ++f) {
            p_pow *= p;
            for (size_t j = 0; j < base_sz; ++j) {
                divs[sz++] = divs[j] * (seq_t)p_pow;
            }
        }
    }

    /* Sum floor(d^2/4) for all divisors d. */
    seq_t sum = 0;
    for (size_t i = 0; i < sz; ++i) {
        seq_t d = divs[i];
        sum += (d * d) / 4;        /* integer division truncates correctly */
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. */
    return sum_floor_divisors(n);
}
