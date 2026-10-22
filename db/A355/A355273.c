/*
 * A355273  Primes p for which p + q is a multiple of 4, where q is the
 *          previous prime if p == 2 (mod 3) or the next prime otherwise.
 * https://oeis.org/A355273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 3425, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A355273: primes p for which p + q is a multiple of 4,
   where q is the previous prime if p ≡ 2 (mod 3) or the next prime otherwise.
   The function returns the n‑th such prime (1‑based index). */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/next_prime.h"
#include "../utils/prev_prime.h"

static seq_t *terms   = NULL;   /* stored sequence terms */
static size_t   cnt    = 0;      /* number of terms stored */
static size_t   cap    = 0;      /* current allocation size */
static idx_t    last_p = 2;      /* last prime used to generate the next one */

static void grow_terms(void)
{
    size_t new_cap = (cap == 0) ? 8 : cap * 2;
    seq_t *new_terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) abort();          /* allocation failure */
    terms = new_terms;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) abort(); /* OEIS offset is 1, undefined for n<1 */

    /* Ensure we have at least n terms cached */
    while (cnt < (size_t)n) {
        /* Get next prime p */
        idx_t p_uint = next_prime(last_p);
        seq_t p = (seq_t)p_uint;
        last_p = p_uint;          /* update for the next iteration */

        /* Choose q according to the rule */
        seq_t q;
        if ((p % (seq_t)3) == (seq_t)2) {          /* previous prime */
            idx_t q_uint = prev_prime(p_uint);
            q = (seq_t)q_uint;
        } else {                                 /* next prime */
            idx_t q_uint = next_prime(p_uint);
            q = (seq_t)q_uint;
        }

        /* Check whether p + q is divisible by 4 */
        if (((p + q) & (seq_t)3) == 0) {
            /* Store the qualifying prime */
            if (cnt == cap) grow_terms();
            terms[cnt++] = p;
        }
    }

    return terms[(size_t)n - 1];
}
