/*
 * A373602  Numbers k such that the sum of prime factors (with multiplicity)
 *          of A276086(k) is a multiple of 3, where A276086 is the primorial
 *          base exp-function.
 * https://oeis.org/A373602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k ≥ 0 such that the sum of the prime
 *            factors (with multiplicity) of the primorial‑base exp‑function
 *            A276086(k) is divisible by 3
 * Range:     u8 n <= 83, u16 n <= 9802, u32 n <= 9802, u64 n <= 9802,
 *            u128 n <= 9802, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function increments k in mixed‑radix form (bases are the successive primes),
   keeps the weighted sum Σ d_i · p_i, and checks this sum modulo 3. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* A small static allocation – far more than enough for the required range. */
    const size_t MAX_DIGITS = 128;
    idx_t digits[MAX_DIGITS];
    uint64_t primes[MAX_DIGITS];

    size_t digits_len = 1;          /* number of used digits (≥ 1) */
    primes[0] = 2;                  /* initial prime */
    digits[0] = 0;                  /* start with k = 0 */

    seq_t sum = 0;                   /* Σ d_i * p_i for the current k */
    idx_t k = 0;                     /* current index k */
    idx_t count = 0;                 /* numbers that satisfy the condition */

    while (1) {
        /* Check current k */
        if ((sum % 3) == 0) {
            count++;
            if (count == n)
                return (seq_t)k;    /* found the nth term */
        }

        /* Increment k in primorial‑base representation */
        k++;
        size_t i = 0;
        while (1) {
            /* Does the i‑th digit overflow its base (prime[i])? */
            if (digits[i] + 1 == primes[i]) {
                /* Overflow: remove the existing contribution of this digit */
                sum -= (seq_t)digits[i] * primes[i];
                digits[i] = 0;
                i++;

                /* Grow the representation if we ran out of digits */
                if (i >= digits_len) {
                    primes[digits_len] = (uint64_t)get_prime((idx_t)(digits_len + 1));
                    digits[digits_len] = 0;
                    digits_len++;
                }
                /* Continue the carry to the next digit */
                continue;
            } else {
                /* Normal increment of this digit */
                digits[i] += 1;
                sum += (seq_t)primes[i];
                break;               /* digit increment done, move to next k */
            }
        }
    }
}
