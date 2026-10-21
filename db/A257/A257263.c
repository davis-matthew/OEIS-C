/*
 * A257263  Numbers such that the least missing nonzero digit (A257079) in
 *          their factorial base representation is 3.
 * https://oeis.org/A257263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 9557, u32 n <= 9557, u64 n <= 9557,
 *            u128 n <= 9557, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A257263: numbers whose factorial base representation has
   least missing nonzero digit equal to 3.
   Computation: enumerate integers, test their factorial base digits
   for the presence of 1 and 2 and absence of 3, store results in
   a growing array. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *terms = NULL;          /* array of found terms */
static idx_t  terms_cnt = 0;         /* number of terms cached */
static size_t terms_alloc = 0;       /* allocated capacity of 'terms' */
static idx_t  next_cand  = 1;        /* next candidate integer to test */

seq_t a(idx_t n) {
    /* Return the nth term (offset = 1).  Generate terms on demand. */
    while (terms_cnt < n) {
        seq_t cand = (seq_t)next_cand;
        bool has1 = false, has2 = false, has3 = false;
        seq_t x = cand;
        unsigned long long pos = 1;   /* factorial position (1! for pos=1) */

        while (x > 0) {
            seq_t mult = (seq_t)(pos + 1);
            seq_t d = x % mult;      /* digit in this position */

            if (d == 1) has1 = true;
            else if (d == 2) has2 = true;
            else if (d == 3) { has3 = true; break; }

            x /= mult;
            ++pos;
        }

        if (!has3 && has1 && has2) {
            /* store term */
            if (terms_cnt == terms_alloc) {
                size_t new_alloc = terms_alloc ? terms_alloc * 2 : 1024;
                seq_t *tmp = (seq_t *)realloc(terms, new_alloc * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                terms = tmp;
                terms_alloc = new_alloc;
            }
            terms[terms_cnt++] = cand;
        }
        ++next_cand;
    }
    return terms[n - 1];
}
