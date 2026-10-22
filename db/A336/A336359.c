/*
 * A336359  Numbers k for which bigomega(sigma(k)) < bigomega(k), where
 *          bigomega (A001222) gives the number of prime factors with
 *          multiplicity, and sigma (A000203) gives the sum of divisors.
 * https://oeis.org/A336359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th smallest integer k such that A001222(σ(k)) <
 *            A001222(k), where σ is the sum‑of‑divisors function and A001222
 *            counts prime factors with multiplicity
 * Range:     u8 n <= 16, u16 n <= 1168, u32 n <= 9934, u64 n <= 9934,
 *            u128 n <= 9934, big any n
 * Helpers:   bigomega, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/bigomega.h"
#include "../utils/sigma_of.h"

/* cached list of found terms */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_cnt = 0;

/* highest k that has been examined */
static idx_t last_k = 0;

/* ensure there is room for at least 'need' entries in the terms array */
static void ensure_terms_cap(size_t need)
{
    if (terms_cap >= need) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = tmp;
    terms_cap = new_cap;
}

/*------------------------------------------------------------*/
/*  Main sequence function                                   */
seq_t a(idx_t n)
{
    while (terms_cnt < n) {
        ++last_k;                     /* next candidate k */

        /* compute bigomega(k) and sigma(k) */
        seq_t bigomega_k = bigomega(last_k);
        seq_t sigma      = sigma_of(last_k);

        /* bigomega(sigma(k)) – convert sigma to idx_t.  The values
           involved for the first 10 000 terms fit in 64‑bit, which
           is the length of idx_t.  If sigma is ever larger we
           would need a different method, but for the problem
           requirements this is sufficient. */
        seq_t bigomega_sigma = bigomega((idx_t)sigma);

        /* store qualifying k */
        if (bigomega_sigma < bigomega_k) {
            ensure_terms_cap(terms_cnt + 1);
            terms[terms_cnt++] = (seq_t)last_k;
        }
    }
    return terms[n - 1];
}
