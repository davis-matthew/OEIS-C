/*
 * A354038  Numbers k such that sum of the proper divisors of k is a multiple
 *          of 4.
 * https://oeis.org/A354038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354038:  Numbers k such that the sum of proper divisors of k
   is a multiple of 4.  The code tests each integer in ascending order
   and records those satisfying sigma(k) ≡ k (mod 4). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t  *terms   = NULL;  /* array of sequence values       */
    static size_t  cap     = 0;     /* current storage capacity       */
    static idx_t   len     = 0;     /* number of terms stored so far  */
    static idx_t   next_k  = 1;     /* next integer to test           */

    if (n == 0) return 0;          /* undefined for n<1               */

    /* Generate terms until we have at least n of them. */
    while (len < n)
    {
        uint64_t k = (uint64_t)next_k++;

        /* Compute sigma(k) modulo 4 via multiplicativity. */
        int sigma_mod4 = 1;       /* multiplicative identity    */

        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt;
        factor_u64(k, primes, exps, &cnt);

        for (size_t i = 0; i < cnt; ++i)
        {
            uint64_t p = primes[i];
            unsigned int e = (unsigned int)exps[i];
            int term_mod4;

            if (p == 2) {
                /* sigma(2^e) = 2^(e+1)-1 ≡ 3 (mod 4) for e≥1, 1 if e=0. */
                term_mod4 = (e == 0) ? 1 : 3;
            } else if ((p & 3) == 1) {
                /* p ≡ 1 (mod 4): sigma(p^e) ≡ e+1 (mod 4). */
                term_mod4 = (int)((e + 1) & 3);
            } else { /* p ≡ 3 (mod 4) */
                /* sigma(p^e) ≡ 1 if e even, 0 if e odd. */
                term_mod4 = (e & 1) ? 0 : 1;
            }

            sigma_mod4 = (sigma_mod4 * term_mod4) & 3;
        }

        int k_mod4 = (int)(k & 3);

        if (sigma_mod4 == k_mod4) {    /* k belongs to the sequence */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();     /* allocation failed */
                terms = tmp;
                cap   = new_cap;
            }
            terms[len++] = (seq_t)k;
        }
    }

    return terms[n - 1];
}
