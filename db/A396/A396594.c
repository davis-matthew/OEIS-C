/*
 * A396594  Numbers m such that rad(m) * bigomega(m) = m.
 * https://oeis.org/A396594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 9046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A396594: numbers m such that rad(m) * bigomega(m) = m. */
#include <stdlib.h>
#include <stdint.h>

/* Utilities that provide the required arithmetic functions. */
#include "../utils/rad.h"
#include "../utils/bigomega.h"

/* Dirichlet divisor function (bigomega) and rad are assumed to be
   declared in the corresponding headers. They accept and return values
   of type idx_t/seq_t as used by the build. */

seq_t a(idx_t n)
{
    /* Statically cache the found terms between calls. */
    static seq_t *terms = NULL;     /* array of terms in sequence order   */
    static size_t terms_cap = 0;    /* capacity of the array              */
    static size_t terms_cnt = 0;    /* number of terms stored             */
    static idx_t last_tested = 1;   /* largest integer already checked    */

    /* Helper to grow the terms array. */
    if (terms == NULL) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (terms_cnt < (size_t)n) {
        ++last_tested;                     /* candidate m               */
        seq_t rad_val    = rad(last_tested);            /* rad(m)            */
        seq_t omega_val  = bigomega(last_tested);      /* bigomega(m)      */

        /* Compute rad(m) * bigomega(m) in 128‑bit to avoid overflow. */
        unsigned __int128 prod = (unsigned __int128)rad_val *
                                 (unsigned __int128)omega_val;

        if (prod == (unsigned __int128)last_tested) {
            /* Store the term.  Expand the array if needed. */
            if (terms_cnt == terms_cap) {
                size_t new_cap = terms_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) { free(terms); return 0; } /* safe abort on OOM */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_cnt++] = (seq_t)last_tested;
        }
    }

    return terms[n - 1];
}
