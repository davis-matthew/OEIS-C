/*
 * A274002  Primes p of the form (q-1)^2+1 that are a divisor of 4^(q-1)-1
 *          where q is prime.
 * https://oeis.org/A274002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   append_candidate, is_prime, is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A274002
 * Computation: Search successive prime q. For each prime q compute
 *     p = (q-1)^2 + 1.
 * If p is prime and 4^(q-1) ≡ 1 (mod p) then p is appended to the
 * sequence.  The sequence is zero-indexed internally; a(1) is the
 * first term 5, etc.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"
#include "../utils/append_candidate.h"

/* Compute the nth term of A274002. */
seq_t a(idx_t n)
{
    /* Zero index into the internal array */
    static seq_t *seq = NULL;
    static size_t size = 0;        /* number of elements stored */
    static size_t cap  = 0;        /* allocated capacity */
    static uint64_t last_q = 2;    /* last prime q processed */

    if (n == 0) {
        return (seq_t)0;           /* undefined, but safe default */
    }

    /* Generate terms until we have at least n entries */
    while (size < (size_t)n) {
        for (uint64_t q = last_q + 1; ; ++q) {
            if (!is_prime(q)) {
                /* q is not prime, continue searching */
                continue;
            }

            /* Compute p = (q-1)^2 + 1 in arbitrary precision */
            seq_t q_minus = (seq_t)(q - 1);
            seq_t p = q_minus * q_minus + (seq_t)1;

            /* p must be prime */
            if (!is_prime_big(p)) {
                continue;
            }

            /* Check 4^(q-1) ≡ 1 (mod p) */
            seq_t r = mod_pow((seq_t)4, q - 1, p);
            if (r != (seq_t)1) {
                continue;
            }

            /* All conditions satisfied; add to sequence */
            append_candidate(&seq, &size, &cap, p);
            last_q = q;
            break;                /* next term found, break the for-loop */
        }
    }

    /* Return the requested term (1‑based index to 0‑based array) */
    return seq[n - 1];
}
