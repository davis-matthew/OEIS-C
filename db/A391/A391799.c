/*
 * A391799  Numbers k such that prime(2*k + 2) - prime(2*k) > prime(2*k + 1) -
 *          prime(2*k - 1).
 * https://oeis.org/A391799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that prime(2k+2) - prime(2k) > prime(2k+1) -
 *            prime(2k-1)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   build_next_prime_sequence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_next_prime_sequence.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* harmless value for out-of-range calls */

    size_t k = 1;          /* candidate k */
    size_t found = 0;      /* how many terms we have found */

    /* Local list of primes used during this call.
       It grows as needed but is freed before returning.          */
    uint32_t *primes = NULL;
    size_t primes_len = 0, primes_cap = 0;
    uint32_t next_candidate = 2;

    while (found < (size_t)n) {
        /* Ensure we have primes up to index 2*k+2 (1‑based). */
        size_t need = 2 * k + 2;          /* number of primes required */
        while (primes_len < need) {
            bool is_prime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                uint32_t p = primes[i];
                if ((uint64_t)p * (uint64_t)p > (uint64_t)next_candidate)
                    break;
                if (next_candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                if (primes_len == primes_cap) {
                    primes_cap = primes_cap ? primes_cap << 1 : 16;
                    primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                }
                primes[primes_len++] = next_candidate;
            }
            if (next_candidate == 2)
                next_candidate = 3;
            else if (next_candidate == 3)
                next_candidate = 5;
            else
                next_candidate += 2;
        }

        /* Compute the two prime gaps.  Indices are 0‑based: prime(n) = primes[n-1]. */
        seq_t left  = (seq_t)primes[2 * k + 1] - (seq_t)primes[2 * k - 1];
        seq_t right = (seq_t)primes[2 * k]     - (seq_t)primes[2 * k - 2];

        if (left > right) {
            ++found;
            if (found == (size_t)n) {
                free(primes);
                return (seq_t)k;
            }
        }
        ++k;
    }

    /* Should never reach here, but free memory just in case. */
    free(primes);
    return (seq_t)0;
}
