/*
 * A374472  Numbers k such that bigomega(k) and A064547(k) are both even,
 *          where A064547 is the count of 1-bits in the exponents of the prime
 *          factorization of n, and bigomega is the number of prime factors of
 *          n (with multiplicity).
 * https://oeis.org/A374472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 9990, u32 n <= 9990, u64 n <= 9990,
 *            u128 n <= 9990, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374472:
   a(n) is the n-th positive integer k such that
   bigomega(k) (the total number of prime factors with multiplicity)
   and A064547(k) (the sum of popcount of the exponents in the
   prime factorisation of k) are both even.  The function builds the
   sequence lazily, keeping a static cache of found terms.  It uses
   a sieve of smallest prime factors to factorise numbers quickly. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/build_spf.h"   /* build_spf(idx_t limit, idx_t **pf_out) */

/* --------------------------------------------------------------------- */
/*  Static cache for the sequence                                         */
/* --------------------------------------------------------------------- */
static seq_t *seq      = NULL;   /* sequence values         */
static size_t seq_len  = 0;       /* current number of terms */
static size_t seq_cap  = 0;       /* allocation capacity     */

/* --------------------------------------------------------------------- */
/*  Smallest‑prime‑factor table and its current bound                    */
/* --------------------------------------------------------------------- */
static idx_t *spf      = NULL;   /* spf[0..spf_lim] */
static idx_t  spf_lim  = 0;      /* maximum index for which spf is defined */

/* --------------------------------------------------------------------- */
/*  Highest integer that has been examined so far                         */
/* --------------------------------------------------------------------- */
static idx_t last_checked = 0;

/* --------------------------------------------------------------------- */
/*  Ensure that spf array covers at least 0 .. upto-1                      */
/* --------------------------------------------------------------------- */
static void ensure_spf(idx_t upto)
{
    if (upto <= spf_lim) return;

    idx_t new_lim = spf_lim + 1;
    if (new_lim < 2) new_lim = 2;
    while (new_lim < upto) new_lim <<= 1;   /* double until big enough */

    if (spf) free(spf);
    build_spf(new_lim, &spf);   /* spf[0..new_lim] is now ready   */
    spf_lim = new_lim;
}

/* --------------------------------------------------------------------- */
/*  Append a new term to the sequence array                               */
/* --------------------------------------------------------------------- */
static void seq_append(seq_t val)
{
    if (seq_len >= seq_cap) {
        size_t newcap = seq_cap ? seq_cap * 2 : 16;
        seq = (seq_t*)realloc(seq, newcap * sizeof(seq_t));
        seq_cap = newcap;
    }
    seq[seq_len++] = val;
}

/* --------------------------------------------------------------------- */
/*  Test whether an integer k satisfies the two parity conditions       */
/* --------------------------------------------------------------------- */
static bool candidate_ok(idx_t k)
{
    if (k == 1) return true;   /* 1 is included explicitly */

    idx_t tmp = k;
    int total_pf = 0;       /* bigomega(k)   */
    int total_pop = 0;      /* A064547(k)    */

    while (tmp > 1) {
        idx_t p = spf[tmp];
        int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            exp++;
        }
        total_pf += exp;
        total_pop += __builtin_popcount((unsigned)exp);
    }
    return ((total_pf & 1) == 0) && ((total_pop & 1) == 0);
}

/* --------------------------------------------------------------------- */
/*  Main function that returns the n-th term of the sequence             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0 */

    /* initialise sieve if needed */
    if (spf_lim == 0) ensure_spf(1000);

    /* grow the sequence until it contains at least n terms            */
    while (seq_len < n) {
        last_checked++;
        ensure_spf(last_checked);   /* ensure s.f.p. table is large enough */
        if (candidate_ok(last_checked))
            seq_append((seq_t)last_checked);
    }
    return seq[n - 1];
}
