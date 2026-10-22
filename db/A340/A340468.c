/*
 * A340468  a(n) is the least prime of the form 2 + Product_{i=n..m} prime(i).
 * https://oeis.org/A340468
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime of the form 2 + Π_{i=n..m} prime(i) for some
 *            m ≥ n
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   get_prime, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Utility functions from the build environment */
#include "../utils/is_prime_big.h"
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"

/* seq_t is provided by the build (unsigned integer or arbitrary‑precision).
   idx_t is uint64_t. */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2. */
    if (n < 2) {
        return 0;
    }

    seq_t product = 1;            /* Product of primes from prime(n) onward. */
    for (idx_t m = n; ; ++m) {
        /* Get the m‑th prime.  The utility get_prime is 1‑based. */
        uint32_t p_val = get_prime(m);

        /* Multiply into the product (in seq_t). */
        product *= (seq_t)p_val;

        /* Candidate prime = 2 + product. */
        seq_t candidate = product + 2;
        if (is_prime_big(candidate)) {
            return candidate;
        }

        /* Move to the next prime for the next iteration. */
        /* In case get_prime has limitations, fall back to next_prime. */
        if (m == n) {
            /* Nothing to do – we already have p_val. */
        }
    }

    /* Should never reach here. */
    return 0;
}
