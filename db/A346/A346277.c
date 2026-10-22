/*
 * A346277  Primitive terms of A108569.
 * https://oeis.org/A346277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primitive terms of A108569.
   a(n) is computed by searching odd m (m ≥ 3) for which
   φ(2m) = φ(2m + φ(2m)).  The smallest such k = 2m is a primitive term.
   Known that the first primitive term is 4 (k = 2·2).  Subsequent primitive
   terms correspond to odd m.  The function caches all terms found so far
   so that successive calls are efficient regardless of call order. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/calc_phi.h"

static seq_t *terms = NULL;   /* cache of primitive terms */
static size_t terms_len = 0;  /* number of terms stored      */
static size_t terms_cap = 0;  /* allocated capacity          */
static idx_t next_m = 3;      /* odd m to test next          */

seq_t a(idx_t n)
{
    /* initialise cache with the first term 4 (k = 2·2) */
    if (terms == NULL) {
        terms_cap = 32;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) abort();
        terms[0] = (seq_t)4;
        terms_len = 1;
    }

    /* generate terms until we have at least n entries */
    while (terms_len < (size_t)n) {
        idx_t      m      = next_m;          /* current odd m           */
        next_m += 2;                         /* prepare next odd m      */

        seq_t two_m  = (seq_t)2 * m;         /* k = 2*m                  */
        seq_t phi_m  = calc_phi(two_m);      /* φ(2m)                   */
        seq_t t      = two_m + phi_m;        /* 2m + φ(2m)               */
        seq_t phi_t  = calc_phi(t);          /* φ(2m + φ(2m))           */

        if (phi_t == phi_m) {
            /* ensure capacity for new term */
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 32;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!terms) abort();
            }
            terms[terms_len++] = two_m;
        }
    }

    return terms[(size_t)(n - 1)];
}
