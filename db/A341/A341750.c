/*
 * A341750  Numbers k such that gcd(k, sigma(k)) > log(log(k)).
 * https://oeis.org/A341750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that gcd(k, sigma(k)) > log(log(k))
 * Range:     u8 n <= 149, u16 n <= 8474, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple incremental sieve that records, on
 * demand, the first N terms of the sequence.  It relies on the
 * provided utilities 'sigma_of' and 'gcd'. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Static storage for the sequence values and the scanning state. */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static idx_t next_k = 1;         /* next integer to test for the
                                       property.                  */

    /* Allocate the array on first use. */
    if (terms == NULL) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* Extend the array progressively until we have at least n terms. */
    while (terms_len < n) {
        bool ok;

        /* Determine whether the current candidate satisfies the
         * condition.  The comparison uses the fact that
         *   gcd(k, sigma(k))  >  log(log(k))
         * is equivalent to
         *   gcd(k, sigma(k))  >=  floor(log(log(k))) + 1,
         * because log(log(k)) is never an integer. */
        if (next_k <= 1) {
            ok = true;
        } else {
            seq_t sigma = sigma_of((idx_t)next_k);
            seq_t g     = gcd((seq_t)next_k, sigma);
            double thr  = log(log((double)next_k));
            int thr_floor = (int)thr;             /* floor(log(log(k))) */
            if (thr_floor < 0) {
                ok = true;                       /* log(log(k)) < 0 */
            } else {
                ok = (g >= (seq_t)(thr_floor + 1));
            }
        }

        if (ok) {
            /* Grow the array if necessary. */
            if (terms_len == terms_cap) {
                terms_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)next_k;
        }

        next_k++;   /* move to the next integer */
    }

    return terms[n - 1];
}
