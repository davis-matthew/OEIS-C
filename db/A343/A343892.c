/*
 * A343892  Side b of integer-sided primitive triangles (a, b, c) where side a
 *          is the harmonic mean of the 2 other sides b and c, i.e., 2/a = 1/b
 *          + 1/c with b < a < c.
 * https://oeis.org/A343892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A343892: side b of integer‑sided primitive triangles (a,b,c) where 2/a = 1/b + 1/c. 
   The triples are listed in increasing order of a, then b. */
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    static seq_t *b_arr = NULL;
    static size_t cap = 0, len = 0;
    static idx_t a_cur = 3;
    static idx_t b_cur = 0;

    /* Initialisation on first call */
    if (b_arr == NULL) {
        cap = 128;
        b_arr = (seq_t *)malloc(cap * sizeof(seq_t));
        len   = 0;
        a_cur = 3;
        b_cur = (a_cur / 2) + 1;
    }

    /* Ensure capacity for at least one more element */
    if (cap <= len) {
        size_t new_cap = cap * 2;
        while (new_cap <= len) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* cannot happen in typical use */
        b_arr = tmp;
        cap   = new_cap;
    }

    /* Generate triples until we have at least n terms */
    while (len < n) {
        if (b_cur >= a_cur) {          /* finished current a, move to next a */
            ++a_cur;
            b_cur = (a_cur / 2) + 1;
            continue;
        }

        seq_t A = (seq_t)a_cur;
        seq_t B = (seq_t)b_cur;
        seq_t denom = 2 * B - A;        /* 2*b - a  > 0 because b > a/2 */
        if (denom > 0) {
            seq_t numer = A * B;
            if (numer % denom == 0) {
                seq_t C = numer / denom;
                if (C > A && (A + B) > C) {   /* triangle inequality a + b > c */
                    seq_t g = gcd(A, B);
                    g = gcd(g, C);
                    if (g == 1) {               /* primitive triangle */
                        /* Ensure room for this new term */
                        if (cap <= len) {
                            size_t new_cap = cap * 2;
                            while (new_cap <= len) new_cap *= 2;
                            seq_t *tmp = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
                            if (!tmp) abort();
                            b_arr = tmp;
                            cap   = new_cap;
                        }
                        b_arr[len++] = B;
                    }
                }
            }
        }
        ++b_cur;
    }

    return b_arr[n - 1];
}
