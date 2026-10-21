/*
 * A254868  Recamán [-, +, *]-sequence with seed 6 and step 4.
 * https://oeis.org/A254868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, hear, nonn
 * Formula:   a(1) = 6; for n > 1 a(n) = if a(n-1) > 4, let x = a(n-1) - 4 and
 *            if x not in {a(1)..a(n-1)} then x; else if a(n-1) + 4 not in
 *            {a(1)..a(n-1)} then a(n-1) + 4; else a(n-1) * 4 (guaranteed
 *            new). This implements the Recamán [-, +, *]-sequence with seed 6
 *            and step 4
 * Range:     u8 n <= 54, u16 n <= 8098, u32 n <= 8098, u64 n <= 8098,
 *            u128 n <= 8098, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* helper: linear search for a value in the computed sequence */
static bool contains(const seq_t *terms, size_t len, seq_t val)
{
    for (size_t i = 0; i < len; ++i)
        if (terms[i] == val)
            return true;
    return false;
}

/* Recamán sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* undefined for n < 1 */

    static seq_t *terms = NULL;          /* array of computed terms */
    static size_t terms_len = 0;         /* number of elements stored */
    static size_t terms_cap = 0;         /* capacity of the array */

    static const seq_t seed = (seq_t)6;   /* a(1) */
    static const seq_t step = (seq_t)4;   /* step size */

    /* initialize array with seed on first call */
    if (terms_len == 0) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms[0] = seed;
        terms_len = 1;
    }

    /* compute next terms until we reach requested n */
    while (terms_len < n) {
        seq_t cur = terms[terms_len - 1];
        seq_t cand;
        bool found = false;

        /* try subtraction: only if result stays positive */
        if (cur > step) {
            cand = cur - step;
            if (!contains(terms, terms_len, cand)) {
                found = true;
            }
        }

        /* if subtraction fails, try addition */
        if (!found) {
            cand = cur + step;
            if (!contains(terms, terms_len, cand)) {
                found = true;
            }
        }

        /* if both fail, try multiplication */
        if (!found) {
            cand = cur * step;
            /* multiplication always > 0 for our positive sequence */
            if (!contains(terms, terms_len, cand)) {
                found = true;
            }
        }

        if (!found) {
            /* sequence has terminated before reaching n; still return last known term */
            return terms[terms_len - 1];
        }

        /* ensure capacity */
        if (terms_len == terms_cap) {
            terms_cap = terms_cap ? terms_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            if (!tmp) {               /* allocation failure – return last known term */
                return terms[terms_len - 1];
            }
            terms = tmp;
        }

        /* append the chosen candidate */
        terms[terms_len++] = cand;
    }

    return terms[n - 1];
}
