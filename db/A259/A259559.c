/*
 * A259559  Numbers n such that prime(n)-1 and prime(n+1)-1 have the same
 *          number of prime factors, including repeats.
 * https://oeis.org/A259559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   count_prime_factors, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A259559 – n such that prime(n)-1 and prime(n+1)-1 have
   the same number of prime factors (counting multiplicities).
   Computed by checking consecutive primes. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;      /* array of found indices n */
    static size_t terms_len = 0;     /* number of terms stored   */
    static size_t terms_cap = 0;     /* capacity of the array    */
    static idx_t next_prime_index = 1; /* index of prime to test */

    /* Generate terms until we have the requested count. */
    while (terms_len < (size_t)n)
    {
        /* Get current pair of consecutive primes. */
        uint32_t p_i   = get_prime(next_prime_index);
        uint32_t p_ip1 = get_prime(next_prime_index + 1);

        /* Count prime factors of (p_i - 1) and (p_ip1 - 1). */
        seq_t omega1 = count_prime_factors((seq_t)p_i - (seq_t)1);
        seq_t omega2 = count_prime_factors((seq_t)p_ip1 - (seq_t)1);

        if (omega1 == omega2)
        {
            /* Grow the terms array if necessary. */
            if (terms_len == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();           /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)next_prime_index;
        }

        /* Advance to check the next pair. */
        next_prime_index++;
    }

    return terms[n - 1];
}
