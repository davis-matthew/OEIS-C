/*
 * A332676  Prime numbers p_k such that p_k == 3 (mod 10) and p_(k+1) == 3
 *          (mod 10).
 * https://oeis.org/A332676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 197, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A332676 – primes p_k with p_k ≡ 3 (mod 10) and the next prime p_{k+1} ≡ 3 (mod 10) */
/* The function enumerates primes, keeps the last one seen, and whenever two successive primes both
   are ≡ 3 (mod 10) it records the earlier prime.  It memoises the results so that calls
   may come in arbitrary order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static state survives across calls */
    static bool     initialized = false;
    static seq_t  *term_list   = NULL;
    static size_t   term_cap   = 0;
    static size_t   term_cnt   = 0;

    static uint64_t last_prime = 0;
    static uint64_t cand       = 2;   /* next candidate to test for primality */

    /* initialise storage once */
    if (!initialized) {
        term_cap   = 64;
        term_list  = (seq_t *)malloc(term_cap * sizeof(seq_t));
        if (!term_list) return (seq_t)0;   /* failure to allocate */
        initialized = true;
    }

    /* generate primes until we have at least n terms */
    while (term_cnt < n) {
        /* advance to an odd candidate (cand is always odd after the first iteration) */
        if (cand < 3) {
            cand = 3;
        } else if ((cand & 1) == 0) {
            cand += 1;
        }

        /* find next prime */
        while (!is_prime((idx_t)cand)) {
            cand += 2;               /* skip even numbers */
        }

        /* if the last prime and this prime are both ≡ 3 mod 10, record the last one */
        if (last_prime &&
            (last_prime % 10 == 3) &&
            (cand      % 10 == 3)) {

            if (term_cnt == term_cap) {
                term_cap = (term_cap == 0) ? 64 : term_cap * 2;
                seq_t *tmp = (seq_t *)realloc(term_list, term_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;  /* allocation failure */
                term_list = tmp;
            }
            term_list[term_cnt++] = (seq_t)last_prime;
        }

        /* advance to next prime */
        last_prime = cand;
        cand += 2;   /* next odd candidate */
    }

    return term_list[n - 1];
}
