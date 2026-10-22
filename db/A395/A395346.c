/*
 * A395346  Primes that begin a pair of consecutive primes with the same
 *          number of digits and a prime-valued digit dot product.
 * https://oeis.org/A395346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th prime p such that the next prime q * has the
 *            same number of decimal digits and the digit dot product * p·q =
 *            Σ (digit_i(p) * digit_i(q)) is itself a prime
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   digit_count, is_prime, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t x); */
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */
#include "../utils/digit_count.h"  /* uint64_t digit_count(uint64_t n); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static uint64_t candidate = 2;      /* number to test for primality */

    /* if called with n==0 we return 0 (undefined but harmless) */
    if (n == 0)
        return 0;

    /* Ensure we have at least n terms in the cache */
    while (terms_len < (size_t)n)
    {
        /* Find the next prime candidate ≥ candidate */
        while (!is_prime((idx_t)candidate))
            ++candidate;

        uint64_t p = candidate;
        uint64_t q = next_prime(p);  /* prime immediately following p */

        /* Same number of decimal digits? */
        if (digit_count(p) == digit_count(q))
        {
            /* Compute digit dot product */
            seq_t dot = 0;
            uint64_t ap = p, aq = q;
            while (ap && aq)
            {
                uint8_t da = ap % 10;
                uint8_t db = aq % 10;
                dot += (seq_t)(da * db);
                ap /= 10;
                aq /= 10;
            }

            /* dot is prime? */
            if (is_prime_big(dot))
            {
                /* Grow cache if necessary */
                if (terms_len == terms_cap)
                {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!new_terms)   /* allocation failure – return 0 (unlikely) */
                        return 0;
                    terms = new_terms;
                    terms_cap = new_cap;
                }
                terms[terms_len++] = p;   /* store term */
            }
        }

        /* move candidate beyond the current prime for next iteration */
        ++candidate;
    }

    return terms[n - 1];
}
