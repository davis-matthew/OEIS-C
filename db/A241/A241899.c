/*
 * A241899  Numbers n equal to the sum of all the two-digit numbers formed
 *          without repetition from the digits of n.
 * https://oeis.org/A241899
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241899
   a(n) = the n-th integer that equals the sum of all two‑digit numbers
          formed from the digits of n where each unordered pair of digit
          positions contributes 11*(digit_i + digit_j) if the digits are
          distinct, and 11*digit if they are equal.  The sequence has
          offset 10 (a(10) = 11). */

/* A small helper that builds the full list of terms once.  The list is
   stored in a static array and looked up by index. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_digits.h"

seq_t a(idx_t n)
{
    /* offsets and indices above the offset are irrelevant */
    if (n < 10)
        return 0;

    /* static state to cache the precomputed terms */
    static bool initialized = false;
    static seq_t *terms = NULL;
    static size_t term_count = 0;

    if (!initialized) {
        /* temporary array to gather terms */
        size_t cap = 16, len = 0;
        terms = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!terms) return 0; /* out of memory; return 0 */

        /* iterate over all 2‑4 digit numbers – the sequence is finite
           and bounded by 9999 as shown by the inequality in the OEIS
           entry. */
        for (idx_t x = 10; x <= 9999; ++x) {
            uint8_t digs[20];
            size_t dlen;
            get_digits((seq_t)x, digs, &dlen);

            /* count digits and their sum */
            unsigned freq[10] = {0};
            unsigned S = 0;
            for (size_t i = 0; i < dlen; ++i) {
                uint8_t d = digs[i];
                freq[d]++;
                S += d;
            }

            /* compute D = Σ_v C(cnt_v,2) * v */
            unsigned D = 0;
            for (int v = 0; v < 10; ++v) {
                if (freq[v] > 1) {
                    D += (freq[v] * (freq[v] - 1) / 2) * v;
                }
            }

            /* sum of all two‑digit numbers formed as described */
            unsigned long long term = 11ULL * (dlen - 1) * S;
            term -= 11ULL * D;

            if (term == (unsigned long long)x) {
                /* store term */
                if (len == cap) {
                    cap *= 2;
                    seq_t *tmp = (seq_t*)realloc(terms, cap * sizeof(seq_t));
                    if (!tmp) {
                        free(terms);
                        return 0; /* allocation failure */
                    }
                    terms = tmp;
                }
                terms[len++] = (seq_t)x;
            }
        }
        term_count = len;
        initialized = true;
    }

    /* convert the index to the table offset */
    size_t pos = (size_t)(n - 10);
    if (pos >= term_count)
        return 0; /* undefined beyond the last term */
    return terms[pos];
}
