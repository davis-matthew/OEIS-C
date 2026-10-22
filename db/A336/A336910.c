/*
 * A336910  Numbers k > 2 such that omega(k) > log(log(k)) + 2 *
 *          sqrt(log(log(k))), where omega(k) is the number of distinct primes
 *          dividing k (A001221).
 * https://oeis.org/A336910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A336910 by sieving integers and testing the inequality
    ω(k) > log(log(k)) + 2 * sqrt(log(log(k))).  The sequence terms are
    generated once and memoised in a static array, so repeated calls
    are fast.  No recursion or hard‑coded values are used. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_spf.h"

#define MAX_TERMS 20000   /* enough for n ≤ 10000 */

static seq_t  terms[MAX_TERMS];
static size_t terms_len = 0;   /* number of terms stored so far */
static idx_t  prev_limit = 2;  /* last integer that has been examined */

static void append_term(seq_t val)
{
    if (terms_len >= MAX_TERMS) abort();
    terms[terms_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) abort();   /* sequence has offset 1 */

    /*  Generate terms until we have at least the nth one.  */
    while (terms_len < (size_t)n) {
        /*  Double the range, but start with a small initial limit.  */
        idx_t new_limit = (prev_limit < 1024) ? 1024 : prev_limit * 2;

        /*  Build smallest prime factor table for [0..new_limit].  */
        idx_t *spf = NULL;
        build_spf(new_limit, &spf);
        if (!spf) abort();

        /*  Compute ω(k) for 0 ≤ k ≤ new_limit.  */
        unsigned char *omega = (unsigned char *)malloc((size_t)(new_limit + 1));
        if (!omega) abort();
        omega[0] = omega[1] = 0;
        for (idx_t i = 2; i <= new_limit; ++i) {
            idx_t p = spf[i];
            idx_t j = i / p;
            if (j % p == 0)
                omega[i] = omega[j];
            else
                omega[i] = omega[j] + 1;
        }

        /*  Scan the newly covered range and collect qualifying terms.  */
        for (idx_t k = prev_limit + 1; k <= new_limit; ++k) {
            if (k <= 2) continue;          /* skip forbidden values */
            double loglog = log(log((double)k));
            double rhs   = loglog + 2.0 * sqrt(loglog);
            if ((double)omega[k] > rhs)
                append_term((seq_t)k);
        }

        /*  Clean up and advance the limit.  */
        free(omega);
        free(spf);
        prev_limit = new_limit;
    }

    return terms[n - 1];
}
