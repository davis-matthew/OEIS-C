/*
 * A259645  Numbers m such that m^2 + 1, 3*m - 1 and m^2 + m + 41 are all
 *          prime.
 * https://oeis.org/A259645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 798, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Search for terms of OEIS A259645.
   a(n) is the nth integer m ≥ 1 such that m²+1, 3·m−1, and m²+m+41 are all prime. */

seq_t a(idx_t n)
{
    static seq_t *terms     = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static idx_t  next_m    = 1;          /* next candidate to test */

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        seq_t m = (seq_t)next_m;
        bool ok = false;

        if (next_m == (idx_t)1) {
            /* special case m = 1 */
            seq_t v1 = m * m + (seq_t)1;
            seq_t v2 = (seq_t)3 * m - (seq_t)1;
            seq_t v3 = m * m + m + (seq_t)41;
            if (isPrimeGeneric(v1) && isPrimeGeneric(v2) && isPrimeGeneric(v3))
                ok = true;
            next_m = 2;                     /* subsequent candidates are even */
        } else {
            /* m is even (all m > 1 must be even for m²+1 to be prime) */
            seq_t v1 = m * m + (seq_t)1;
            seq_t v2 = (seq_t)3 * m - (seq_t)1;
            seq_t v3 = m * m + m + (seq_t)41;
            if (isPrimeGeneric(v1) && isPrimeGeneric(v2) && isPrimeGeneric(v3))
                ok = true;
            next_m += (idx_t)2;              /* skip to next even number */
        }

        if (ok) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp)
                    return (seq_t)0;           /* out‑of‑memory; caller may handle */
                terms     = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = m;
        }
    }

    return terms[n - 1];
}
