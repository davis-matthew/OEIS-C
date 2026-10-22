/*
 * A380745  a(0) = 0; a(n) = the number of times a(n-1) has the same digits in
 *          common with a previous term, in any permutation.
 * https://oeis.org/A380745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(1)=0;
   for n≥2, a(n) = number of indices i, 0≤i≤n-2, such that
   the multiset of decimal digits of a(i) equals that of a(n-1). */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"   /* void digit_counts(seq_t x, unsigned char cnt[10]); */
#include "../utils/seq_realloc.h"   /* seq_t *seq_realloc(seq_t*, size_t, size_t); */

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *seq        = NULL;     /* array of sequence values          */
    static unsigned char *digit_raw = NULL;  /* raw storage of 10‑byte vectors    */
    static unsigned char (*digit)[10]   = NULL;
    static size_t cap  = 0;              /* capacity of arrays                */
    static size_t sz   = 0;              /* number of terms already computed  */

    /* Ensure arrays can hold index n (inclusive) */
    if (sz <= (size_t)n) {
        size_t needed = (size_t)n + 1;
        if (cap < needed) {
            size_t new_cap = cap ? cap << 1 : 16;
            while (new_cap < needed) new_cap <<= 1;

            /* Reallocate the seq_t array using the provided helper */
            seq = seq_realloc(seq, cap, new_cap);
            /* Reallocate the raw digit array (plain POD) */
            digit_raw = (unsigned char *)
                         realloc(digit_raw, new_cap * 10 * sizeof(unsigned char));
            digit = (unsigned char (*)[10])digit_raw;
            cap   = new_cap;
        }

        /* Compute missing terms up to n */
        while (sz <= (size_t)n) {
            if (sz == 0) {                     /* a(0) */
                seq[0] = (seq_t)0;
                unsigned char cnt[10] = {0};
                digit_counts(seq[0], cnt);
                memcpy(digit[0], cnt, 10 * sizeof(unsigned char));
                ++sz;
            } else if (sz == 1) {              /* a(1) */
                seq[1] = (seq_t)0;
                unsigned char cnt[10] = {0};
                digit_counts(seq[1], cnt);
                memcpy(digit[1], cnt, 10 * sizeof(unsigned char));
                ++sz;
            } else {                           /* general case */
                /* multiset of digits of a(n-1) */
                unsigned char key[10];
                memcpy(key, digit[sz - 1], 10 * sizeof(unsigned char));

                /* count earlier terms with the same multiset */
                seq_t count = (seq_t)0;
                for (size_t i = 0; i + 1 < sz; ++i) {
                    if (memcmp(digit[i], key, 10 * sizeof(unsigned char)) == 0) {
                        ++count;
                    }
                }
                seq[sz] = count;

                /* store digit multiset of the new term */
                unsigned char cnt[10] = {0};
                digit_counts(seq[sz], cnt);
                memcpy(digit[sz], cnt, 10 * sizeof(unsigned char));
                ++sz;
            }
        }
    }

    return seq[(size_t)n];
}
