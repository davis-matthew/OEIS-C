/*
 * A248738  Least number m such that both m^2 -/+ prime(n) are (positive)
 *          primes.
 * https://oeis.org/A248738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 23, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the least m such that m^2 ± prime(n) are primes. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Dynamic list of the first primes seen so far. */
static uint32_t *prime_list   = NULL;
static idx_t     prime_count  = 0;  /* number of primes stored */
static idx_t     prime_start  = 2;  /* next candidate to test */

/* Make sure we have at least <target> primes in prime_list. */
static void extend_primes_to(idx_t target)
{
    while (prime_count < target) {
        /* Find the next prime starting from prime_start. */
        idx_t cand = prime_start;

        /* Skip even numbers greater than 2. */
        if (cand > 2 && (cand & 1) == 0) cand++;

        for (;; ++cand) {
            if (cand > 2 && (cand & 1) == 0) continue;
            if (is_prime_big((seq_t)cand)) {
                /* Grow the array if necessary. */
                uint32_t *tmp = (uint32_t *)realloc(prime_list,
                                                    sizeof(uint32_t) * (prime_count + 1));
                if (!tmp) exit(1);  /* unrecoverable */
                prime_list = tmp;
                prime_list[prime_count++] = (uint32_t)cand;
                prime_start = cand + 1;
                break;
            }
        }
    }
}

/* Return the least integer m such that m^2 – p and m^2 + p are primes,
   where p is the n-th prime (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, but return 0 */

    /* Ensure we have the n‑th prime. */
    extend_primes_to(n);
    uint32_t p = prime_list[n - 1];

    /* Search for the smallest m. */
    for (idx_t m = 2; ; ++m) {
        seq_t m2   = (seq_t)m * (seq_t)m;
        if (m2 <= (seq_t)p) continue;          /* m^2 – p must be positive */

        seq_t diff = m2 - (seq_t)p;             /* m^2 – p   */
        seq_t sum  = m2 + (seq_t)p;             /* m^2 + p   */
        if (is_prime_big(diff) && is_prime_big(sum))
            return (seq_t)m;
    }
}
