/*
 * A253851  Mersenne primes (A000668) of the form 2^sigma(n) - 1 for some n.
 * https://oeis.org/A253851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   cmp_seq_t, grow_seq, is_prime_big, pow2, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A253851: Mersenne primes of the form 2^sigma(n)-1, with sigma(n) prime.
   The function searches successive n to collect the required number of such primes,
   stores them, sorts once, then returns the nth smallest value. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/sigma_of.h"          /* seq_t sigma_of(idx_t n); */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t x); */
#include "../utils/pow2.h"              /* seq_t pow2(idx_t exp); */
#include "../utils/grow_seq.h"          /* void grow_seq(seq_t **arr, size_t *cap, size_t need); */
#include "../utils/cmp_seq_t.h"         /* int cmp_seq_t(const void *a, const void *b); */

static seq_t *mersenne_list = NULL;   /* array of found Mersenne primes   */
static size_t mersenne_count = 0;      /* number of primes stored          */
static size_t mersenne_cap   = 0;      /* current capacity of the array    */
static idx_t   next_m        = 1;      /* next integer to test for sigma   */
static bool    sorted        = false;  /* true once the list has been sorted */

seq_t a(idx_t n)
{
    /* Undefined position; not used by the OEIS entry. */
    if (n == 0) return 0;

    /* Generate terms until we have at least n of them. */
    while (mersenne_count < (size_t)n) {
        /* Evaluate sigma(next_m). */
        seq_t sigma = sigma_of(next_m);

        /* sigma must be a prime number. */
        if (is_prime_big(sigma)) {
            idx_t p = (idx_t)sigma;

            /* Compute the candidate Mersenne prime 2^p - 1. */
            seq_t mers = pow2(p);
            mers = mers - (seq_t)1;

            /* The candidate must itself be prime. */
            if (is_prime_big(mers)) {
                /* Reject duplicates (should never happen in practice). */
                bool dup = false;
                for (size_t i = 0; i < mersenne_count; ++i) {
                    if (mersenne_list[i] == mers) {
                        dup = true;
                        break;
                    }
                }
                if (!dup) {
                    /* Ensure capacity and store the new prime. */
                    grow_seq(&mersenne_list, &mersenne_cap, mersenne_count + 1);
                    mersenne_list[mersenne_count++] = mers;
                }
            }
        }
        ++next_m;   /* Advance to the next integer. */
    }

    /* Sort the collection once; subsequent calls use the sorted order. */
    if (!sorted) {
        qsort(mersenne_list, mersenne_count, sizeof(seq_t), cmp_seq_t);
        sorted = true;
    }

    /* Return the (n‑th) smallest Mersenne prime of the searched form. */
    return mersenne_list[(size_t)n - 1];
}
