/*
 * A324388  If n is a prime power (in A000961), then a(n) = n, otherwise a(n)
 *          is the greatest proper unitary divisor of n.
 * https://oeis.org/A324388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 106 terms through n = 9877 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A324388
   Definition:
   If n is a prime power (including 1), a(n) = n.
   Otherwise a(n) is the greatest proper unitary divisor of n.
   The greatest proper unitary divisor is n divided by the smallest
   prime‑power factor of n. */
#include <stdint.h>
#include <stdbool.h>

static seq_t smallest_prime_power(idx_t n)
{
    if (n <= 1)
        return (seq_t)n;          /* not used for n=1 but safe */

    idx_t temp = n;
    seq_t min_pp = 0;
    bool first = true;
    idx_t distinct_primes = 0;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            ++distinct_primes;
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t pp = 1;
            for (idx_t i = 0; i < exp; ++i)
                pp *= (seq_t)p;
            if (first || pp < min_pp) {
                min_pp = pp;
                first   = false;
            }
        }
    }

    if (temp > 1) {                 /* remaining prime factor */
        ++distinct_primes;
        seq_t pp = (seq_t)temp;     /* power = 1 */
        if (first || pp < min_pp) {
            min_pp = pp;
            first   = false;
        }
    }

    /* If only one distinct prime factor, n is a prime power */
    if (distinct_primes <= 1)
        return (seq_t)n;

    return min_pp;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;             /* 1 is treated as a prime power */

    seq_t sp = smallest_prime_power(n);

    /* If sp == n, n is a prime power */
    if (sp == (seq_t)n)
        return (seq_t)n;

    return (seq_t)n / sp;
}
