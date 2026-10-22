/*
 * A355170  The composite numbers in order of appearance in A173395.
 * https://oeis.org/A355170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth composite number that appears when reading the
 *            multiplication table of positive integers starting with 2 along
 *            the antidiagonals. This function implements a direct generation
 *            of the sequence as defined in OEIS A355170
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>   /* for memset */

seq_t a(idx_t n) {
    /* static state to allow out‑of‑order requests */
    static seq_t *terms = NULL;          /* computed sequence values */
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    static seq_t *seen  = NULL;          /* list of seen products */
    static size_t seen_len = 0;
    static size_t seen_cap = 0;

    /* current diagonal (i + j)   */
    static idx_t cur_sum = 4;            /* 2+2 */
    static idx_t cur_i   = 2;            /* start with i = 2 */

    /* ensure at least n terms have been generated */
    while (terms_len < n) {
        /* move to next diagonal if this one is exhausted */
        if (cur_i > cur_sum - 2) {
            cur_sum++;
            cur_i = 2;
            continue;
        }

        /* product of the current pair (i, j) where j = cur_sum - i */
        seq_t prod = (seq_t)cur_i * (seq_t)(cur_sum - cur_i);

        /* check if prod is already seen */
        bool already = false;
        for (size_t k = 0; k < seen_len; ++k) {
            if (seen[k] == prod) {
                already = true;
                break;
            }
        }

        if (!already) {
            /* add to seen list */
            if (seen_len == seen_cap) {
                size_t new_cap = seen_cap ? seen_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(seen, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                seen = tmp;
                seen_cap = new_cap;
            }
            seen[seen_len++] = prod;

            /* add to terms list */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = prod;
        }

        /* advance to next i on the current diagonal */
        cur_i++;
    }

    /* indices are 1‑based in the OEIS definition */
    return terms[n - 1];
}
