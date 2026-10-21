/*
 * A289250  Primes p such that p + 4 is a semiprime.
 * https://oeis.org/A289250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 2104, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p + 4 is a semiprime.
   The function generates primes in ascending order and checks
   whether p+4 factors into exactly two primes (possibly equal). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/build_spf.h"
#include "../utils/append_candidate.h"

/* ------------------------------------------------------------------ */
/*  Static state                                                    */
/* ------------------------------------------------------------------ */
static idx_t *spf          = NULL;   /* Smallest prime factor table */
static idx_t  spf_limit    = 0;      /* Current size of spf array     */
static idx_t  cur_test     = 2;      /* Next integer to examine      */

static seq_t *seq          = NULL;   /* Cached sequence values        */
static size_t seq_len      = 0;      /* Number of terms stored        */
static size_t seq_cap      = 0;      /* Capacity of seq array         */
static bool    init_done   = false;  /* Have we initialised?          */

/* ------------------------------------------------------------------ */
/*  Initialise the sieve and static arrays.                           */
static void init_sequence(void)
{
    /* Start with a modest limit; will grow if required.         */
    spf_limit = 100000;
    build_spf(spf_limit, (idx_t **)&spf);
    cur_test  = 2;

    seq     = NULL;
    seq_len = 0;
    seq_cap = 0;
    init_done = true;
}

/* ------------------------------------------------------------------ */
/*  Extend the sequence until it contains at least n terms.          */
static void extend_sequence(idx_t n)
{
    while (seq_len < (size_t)n) {
        /* Ensure the SPF table covers cur_test and cur_test + 4. */
        if (cur_test + 4 > spf_limit) {
            idx_t new_limit = spf_limit * 2;
            if (new_limit < cur_test + 4)
                new_limit = cur_test + 4 + 1000;          /* safety margin */

            free(spf);
            build_spf(new_limit, (idx_t **)&spf);
            spf_limit = new_limit;
        }

        /* Check whether cur_test is prime. */
        if (spf[cur_test] == cur_test) {
            idx_t m = cur_test + 4;
            idx_t a1 = spf[m];          /* smallest prime factor of m   */
            if (a1 > 1) {               /* m is composite               */
                idx_t rem = m / a1;     /* second factor                 */
                if (rem > 1 && spf[rem] == rem) {
                    /* m has exactly two prime factors (rem is prime). */
                    append_candidate(&seq, &seq_len, &seq_cap, (seq_t)cur_test);
                }
            }
        }
        ++cur_test;
    }
}

/* ------------------------------------------------------------------ */
/*  Public routine                                                     */
seq_t a(idx_t n)
{
    if (!init_done)
        init_sequence();

    if (n == 0)
        return (seq_t)0;                    /* undefined for offset 0 */

    extend_sequence(n);
    return seq[n - 1];
}
