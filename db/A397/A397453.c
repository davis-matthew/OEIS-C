/*
 * A397453  Numbers k such that, writing m = floor(sqrt(k)), both k-m^2 and
 *          k+m^2 are perfect squares.
 * https://oeis.org/A397453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 125, u64 n <= 125,
 *            u128 n <= 125, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Compute A397453 by iterating over m = floor(sqrt(k)).
   For each m we check whether (t+1)^2 - 2*m^2 is a positive
   square s^2 not exceeding 2*m; if so, k = m^2 + s^2 is a term. */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t *terms = NULL;          /* cached sequence values */
static size_t terms_len = 0;         /* number of cached values */
static size_t terms_cap = 0;         /* allocated capacity for `terms` */
static idx_t m_processed = 0;        /* last m that was examined */

/* Append a new term to the cache, growing the array as needed. */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* unused but safety */

    /* Generate terms until we have at least n of them */
    while (terms_len < n) {
        /* process next m */
        ++m_processed;
        seq_t m = (seq_t)m_processed;

        /* T = 2*m^2 */
        seq_t T = (seq_t)2 * m * m;

        /* t = floor(sqrt(T)) */
        seq_t t = floor_sqrt(T);

        /* diff = (t+1)^2 - T  (will be >0 if there is a candidate) */
        seq_t diff = (t + 1) * (t + 1) - T;

        /* candidate must satisfy 0 < diff <= 2*m and be a perfect square */
        if (diff > 0 && diff <= (seq_t)2 * m) {
            seq_t s = floor_sqrt(diff);
            if (s * s == diff) {
                /* k = m^2 + s^2 */
                seq_t k = m * m + diff;
                append_term(k);
            }
        }
    }

    return terms[n - 1];
}
