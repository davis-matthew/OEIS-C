/*
 * A333122  Numbers m such that m = prime(k) + prime(k+5) = prime(k+1) +
 *          prime(k+4) for some k.
 * https://oeis.org/A333122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 430, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A333122: a(n) = number m such that
   prime(k)+prime(k+5) = prime(k+1)+prime(k+4)
   for some k.  The terms are generated in increasing
   order by increasing k.  The function is memoised.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/grow_seq.h"

static seq_t *terms = NULL;     /* array of found terms          */
static size_t terms_cap = 0;    /* current capacity of 'terms'   */
static size_t terms_len = 0;    /* number of terms stored        */
static idx_t next_k = 3;        /* next k to test (starting at 3) */

/* Ensure that terms[0..needed-1] are computed. */
static void ensure_terms(size_t needed)
{
    if (terms_len >= needed)
        return;

    /* enlarge storage as needed */
    grow_seq(&terms, &terms_cap, needed);

    /* generate terms until we have enough */
    while (terms_len < needed) {
        /* fetch primes needed for current k (indices are 1‑based) */
        seq_t pk   = (seq_t)get_prime(next_k);
        seq_t pk1  = (seq_t)get_prime(next_k + 1);
        seq_t pk4  = (seq_t)get_prime(next_k + 4);
        seq_t pk5  = (seq_t)get_prime(next_k + 5);

        seq_t s1 = pk + pk5;   /* prime(k) + prime(k+5)  */
        seq_t s2 = pk1 + pk4;  /* prime(k+1) + prime(k+4) */

        if (s1 == s2)
            terms[terms_len++] = s1;

        ++next_k;
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based; ignore calls with n==0 for safety */
    if (n == 0)
        return 0;

    ensure_terms((size_t)n);
    return terms[n - 1];
}
