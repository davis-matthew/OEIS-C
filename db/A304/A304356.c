/*
 * A304356  Primes that are the sum of three consecutive primes == 1 (mod 4).
 * https://oeis.org/A304356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime that equals the sum of * three
 *            consecutive primes each ≡ 1 (mod 4)
 * Range:     u8 n <= 5, u16 n <= 342, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* -----------------------------------------------------------------------
     *  Static state used for memoisation and incremental generation.
     * ----------------------------------------------------------------------- */
    static idx_t next_candidate = 2;       /* next integer to test for primality   */
    static idx_t *prime_list = NULL;       /* indices of primes ≡ 1 (mod 4)        */
    static size_t prime_cnt = 0;           /* number of stored primes              */
    static size_t prime_cap = 0;           /* capacity of prime_list               */

    static seq_t *term_list = NULL;        /* resulting sequence terms             */
    static size_t term_cnt = 0;            /* number of stored terms               */
    static size_t term_cap = 0;            /* capacity of term_list                */

    static size_t triple_start = 0;        /* start index of the current triple    */

    /* -----------------------------------------------------------------------
     *  Generate terms until we have at least n of them.
     * ----------------------------------------------------------------------- */
    while (term_cnt < (size_t)n)
    {
        /* Make sure we have enough primes to form the triple.   */
        while (prime_cnt < triple_start + 3)
        {
            /* Seek next prime number. */
            do { ++next_candidate; } while (!is_prime(next_candidate));

            /* Keep only primes ≡ 1 (mod 4). */
            if ((next_candidate & 3U) == 1U)
            {
                if (prime_cnt >= prime_cap)
                {
                    prime_cap = prime_cap ? prime_cap << 1 : 4;
                    prime_list = (idx_t *)realloc(prime_list,
                                                  prime_cap * sizeof(idx_t));
                }
                prime_list[prime_cnt++] = next_candidate;
            }
        }

        /* Sum the current triple of qualifying primes. */
        idx_t p1 = prime_list[triple_start];
        idx_t p2 = prime_list[triple_start + 1];
        idx_t p3 = prime_list[triple_start + 2];
        idx_t sum = p1 + p2 + p3;

        /* If the sum itself is prime, record it as a term. */
        if (is_prime(sum))
        {
            seq_t val = (seq_t)sum;
            if (term_cnt >= term_cap)
            {
                term_cap = term_cap ? term_cap << 1 : 4;
                term_list = (seq_t *)realloc(term_list,
                                             term_cap * sizeof(seq_t));
            }
            term_list[term_cnt++] = val;
        }

        /* Advance to the next set of three consecutive qualifying primes. */
        ++triple_start;
    }

    /* Return the requested term (OEIS indices start at 1). */
    return term_list[(size_t)(n - 1)];
}
