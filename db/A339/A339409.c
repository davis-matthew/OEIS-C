/*
 * A339409  Number of compositions (ordered partitions) of n into an odd
 *          number of primes.
 * https://oeis.org/A339409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 33, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A339409
   Formula (generating function): (1/2)*(1/(1 - Σx^prime) - 1/(1 + Σx^prime))
   We compute a(n) = number of compositions of n into an odd number of prime parts. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition (empty sum has 0 parts, which is even). */
    if (n == 0) {
        return 0;
    }

    /* Allocate arrays for counts with odd and even lengths. */
    seq_t *O = mem_new(seq_t, (size_t)(n + 1));
    seq_t *E = mem_new(seq_t, (size_t)(n + 1));
    if (!O || !E) {
        mem_free(O); mem_free(E);
        return 0;
    }

    /* Base cases */
    E[0] = 1;          /* empty composition has 0 parts (even) */
    O[0] = 0;          /* no odd-part composition of 0 */
    for (idx_t i = 1; i <= n; ++i) {
        E[i] = 0;
        O[i] = 0;
    }

    /* Primality sieve up to n. */
    bool *is_composite = (bool*)calloc((size_t)(n + 1), sizeof(bool));
    if (!is_composite) {
        mem_free(O); mem_free(E);
        return 0;
    }

    size_t primes_capacity = n / 2 + 2; /* rough upper bound */
    uint32_t *primes = (uint32_t*)malloc(primes_capacity * sizeof(uint32_t));
    if (!primes) {
        free(is_composite); mem_free(O); mem_free(E);
        return 0;
    }
    size_t primes_count = 0;

    for (idx_t i = 2; i <= n; ++i) {
        if (!is_composite[(size_t)i]) {
            primes[primes_count++] = (uint32_t)i;
            if (i * i <= n) {
                for (idx_t j = i * i; j <= n; j += i) {
                    is_composite[(size_t)j] = true;
                }
            }
        }
    }

    /* Dynamic programming: add a prime to a composition of smaller sum. */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t odd_sum = 0;
        seq_t even_sum = 0;
        for (size_t pi = 0; pi < primes_count && primes[pi] <= i; ++pi) {
            idx_t p = (idx_t)primes[pi];
            odd_sum  += E[i - p];   /* append prime to an even-length composition */
            even_sum += O[i - p];   /* append prime to an odd-length composition */
        }
        O[i] = odd_sum;
        E[i] = even_sum;
    }

    seq_t result = O[n];

    /* Free memory */
    free(is_composite);
    free(primes);
    mem_free(O);
    mem_free(E);
    return result;
}
