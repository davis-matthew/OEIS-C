/*
 * A381805  Smallest composite squarefree number that is coprime to n.
 * https://oeis.org/A381805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p*q, where p and q are the two smallest primes coprime to
 *            n (product of the first two primes not dividing n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

/* cache of primes generated so far -------------------------------------*/
static uint64_t *prime_cache = NULL;
static size_t prime_cnt   = 0;   /* number of primes stored   */
static size_t prime_cap   = 0;   /* allocated capacity        */

/* enlarge the prime cache ----------------------------------------------*/
static void grow_prime_cache(void)
{
    size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                       new_cap * sizeof(uint64_t));
    prime_cache = tmp;
    prime_cap   = new_cap;
}

/* generate the next prime after the last cached one --------------------*/
static uint64_t get_next_prime(void)
{
    uint64_t cand = (prime_cnt == 0) ? 2ULL : prime_cache[prime_cnt - 1] + 1U;

    while (!is_prime(cand))
        ++cand;

    if (prime_cnt == prime_cap)
        grow_prime_cache();

    prime_cache[prime_cnt++] = cand;
    return cand;
}

/* return the next prime that is greater than the last cached one         */
static uint64_t next_prime(void)
{
    return get_next_prime();
}

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    uint64_t p = 0, q = 0;   /* the two primes coprime to n          */
    size_t idx = 0;          /* index into prime_cache                */

    for (;; ++idx) {
        uint64_t cand;
        if (idx < prime_cnt)
            cand = prime_cache[idx];
        else
            cand = next_prime();     /* generate and cache new prime      */

        /* test coprimeness with the index n                     */
        if ( (n % cand) != 0 ) {
            if (p == 0)
                p = cand;
            else {
                q = cand;
                break;                  /* found the two primes            */
            }
        }
    }

    return (seq_t)p * (seq_t)q;
}
