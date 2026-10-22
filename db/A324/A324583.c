/*
 * A324583  Numbers k such that k and A276086(k) are coprime, where A276086 is
 *          the primorial base exp-function.
 * https://oeis.org/A324583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 166, u16 n <= 9954, u32 n <= 9954, u64 n <= 9954,
 *            u128 n <= 9954, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A324583: numbers k such that k and A276086(k) are coprime.
   A276086(k) = Π p_i^{d_i} where k is written in primorial base
   with bases (2,3,5,7,…) and digits d_i < p_i.  k and A276086(k)
   are coprime iff for every i with d_i > 0 we have p_i ∤ k. */

/* forward declarations */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t num) */

/* test whether a given k satisfies the coprimality condition */
static bool coprime_k(seq_t k)
{
    if (k == 0) 
        return true;   /* by definition gcd(0,1)=1 */

    seq_t tmp = k;
    size_t i = 0;

    /* small prime cache (enough for the few primes used in representation) */
    static uint64_t primes[128];
    static size_t primes_cnt = 0;
    static uint64_t next_candidate = 1;        /* will become 2 on first use */

    /* keep expanding the cache until we have the i‑th prime */
    while (primes_cnt <= i)
    {
        ++next_candidate;
        /* simple primality test for the small number next_candidate */
        while (!is_prime_small((seq_t)next_candidate))
            ++next_candidate;
        primes[primes_cnt++] = next_candidate;
    }

    /* iterate through digits of k in primorial base */
    while (tmp > 0)
    {
        seq_t digit = tmp % (seq_t)primes[i];
        tmp     /= (seq_t)primes[i];

        if (digit > 0)
        {
            /* when prime divides k, coprime fails */
            if (k % (seq_t)primes[i] == 0)
                return false;
        }
        ++i;

        /* ensure primes cache is available for next iteration */
        while (primes_cnt <= i)
        {
            ++next_candidate;
            while (!is_prime_small((seq_t)next_candidate))
                ++next_candidate;
            primes[primes_cnt++] = next_candidate;
        }
    }
    return true;
}

/* Memoized generator of the sequence */
seq_t a(idx_t n)
{
    static idx_t next_k = 0;          /* next candidate to test, 0‑based */
    static seq_t *table = NULL;       /* dynamically grown array */
    static size_t cap   = 0;
    static size_t len   = 0;

    while (len < n)
    {
        seq_t k = (seq_t)next_k;
        if (coprime_k(k))
        {
            if (len == cap)          /* grow table */
            {
                size_t new_cap = (cap == 0) ? 16 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failure */
                table = tmp;
                cap   = new_cap;
            }
            table[len++] = k;
        }
        ++next_k;
    }
    return table[n - 1];
}
