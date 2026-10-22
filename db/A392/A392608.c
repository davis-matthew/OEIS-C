/*
 * A392608  Number of nonzero digits in the primorial base representation of n
 *          (A049345) such that their corresponding radix prime divides n.
 * https://oeis.org/A392608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct prime factors p of n such that the *
 *            digit of n in primorial base at radix p is non‑zero. *
 *            Computation: for each prime p, let w = product of all primes <
 *            p. * The digit is (n / w) mod p. Count p iff digit > 0 and p | n
 * Range:     u8 n <= 9830, u16 n <= 9830, u32 n <= 9830, u64 n <= 9830,
 *            u128 n <= 9830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* First 15 primes – sufficient for all n <= 2^64 (idx_t is uint64_t) */
static const uint64_t PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47
};
#define NUM_PRIMES (sizeof(PRIMES)/sizeof(PRIMES[0]))

seq_t a(idx_t n)
{
    /* 0 is the first term */
    if (n == 0)
        return (seq_t)0;

    seq_t ans = (seq_t)0;
    idx_t weight = (idx_t)1;  /* product of primes < current prime */

    for (size_t i = 0; i < NUM_PRIMES; ++i) {
        uint64_t p = PRIMES[i];

        /* If weight already exceeds n, all remaining digits are zero. */
        if (weight > n)
            break;

        /* Digit for prime p */
        idx_t digit = (n / weight) % (idx_t)p;

        /* Count only if digit is non‑zero and p divides n. */
        if (digit != 0 && (n % (idx_t)p) == 0)
            ++ans;

        /* Prevent overflow of weight: stop if the next multiplication
         * would overflow or make weight exceed n (no further non‑zero
         * digits are possible). */
        if (weight > n / (idx_t)p)
            break;

        weight *= (idx_t)p;   /* weight becomes product of primes <= p */
    }

    return ans;
}
