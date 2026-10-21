/*
 * A246066  Least k such that Lppf(k) > Gpf(k) + n, where Lppf(k) is the
 *          largest prime power factor of k and Gpf(k) is the greatest prime
 *          factor of k.
 * https://oeis.org/A246066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that Lppf(k) > Gpf(k) + n. Since a prime
 *            power k = p^e (e ≥ 2) has Lppf(k) = k and Gpf(k) = p, we can
 *            search over all primes p for the smallest k = p^e satisfying p^e
 *            > p + n. The minimal such k is the answer
 * Range:     u8 n <= 66, u16 n <= 3205, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Static storage of generated primes.
   10000 primes are far more than enough for n up to 10000. */
static uint64_t primes[10000];
static size_t primes_count = 0;

/* Helper to ensure the list contains the prime 2 as the first entry. */
static void init_primes(void)
{
    if (primes_count == 0) {
        primes[0] = 2;
        primes_count = 1;
    }
}

/* The sequence function. */
seq_t a(idx_t n)
{
    init_primes();

    seq_t best = (seq_t)(-1);          /* initialise to max value */

    /* Check prime = 2 separately. */
    seq_t p = (seq_t)2;
    seq_t power = p * p;               /* 2^2 */
    seq_t limit = p + (seq_t)n;        /* p + n */
    while (power <= limit) {
        power = power * p;
    }
    best = power;

    /* Generate further primes and test them. */
    for (uint64_t cand = 3;; cand += 2) {
        /* Test cand for primality using the primes already known. */
        bool is_prime = true;
        for (size_t i = 0; i < primes_count; ++i) {
            uint64_t q = primes[i];
            if (q * q > cand) break;
            if (cand % q == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            /* Store the new prime. */
            if (primes_count < sizeof(primes) / sizeof(primes[0])) {
                primes[primes_count++] = cand;
            } else {
                /* Should never happen for the required range. */
                break;
            }

            /* Find the minimal exponent e ≥ 2 with p^e > p + n. */
            seq_t prime = (seq_t)cand;
            seq_t pow = prime * prime;           /* p^2 */
            seq_t lim = prime + (seq_t)n;        /* p + n */
            while (pow <= lim) {
                pow = pow * prime;
            }

            /* Update best if necessary. */
            if (pow < best) {
                best = pow;
            }

            /* Stop if further primes cannot improve best. */
            if (prime * prime >= best) {
                break;
            }
        }
    }

    return best;
}
