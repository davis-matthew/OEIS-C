/*
 * A281004  Numbers with exactly 3 ones in both binary and ternary
 *          representations.
 * https://oeis.org/A281004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 55, u32 n <= 128, u64 n <= 136,
 *            u128 n <= 136, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A281004: numbers with exactly 3 ones in both binary and ternary
   representations (intersection of A014311 and A023694). */

#include <stddef.h>
#include <stdlib.h>

/* Pre‑computed list of terms that fit into seq_t.  The array is large
   enough to hold all known terms; the actual number of stored terms
   is kept in terms_len.  It is built lazily on the first call. */
static seq_t terms[1024];
static size_t terms_len = 0;

/* Comparison function for qsort. */
static int seq_cmp(const void *a, const void *b) {
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    return (*va < *vb) ? -1 : (*va > *vb) ? 1 : 0;
}

/* Build the list of terms that fit into seq_t.  Only numbers with
   exactly three 1–bits in both binary and ternary bases are stored. */
static void init_terms(void) {
    if (terms_len != 0) return;

    /* Determine the maximum value representable by seq_t. */
    seq_t seq_max_val = ~((seq_t)0);
    unsigned __int128 seq_max = (unsigned __int128)seq_max_val;

    size_t idx = 0;

    /* Enumerate all 3‑bit combinations with highest bit up to 40
       (since 2^40 ≈ 1.0995e12 > the largest known term). */
    for (idx_t i = 2; i < 41; ++i) {          /* highest set bit */
        for (idx_t j = 1; j < i; ++j) {      /* middle set bit */
            for (idx_t k = 0; k < j; ++k) {  /* lowest set bit */
                unsigned __int128 v = ((unsigned __int128)1 << i) |
                                      ((unsigned __int128)1 << j) |
                                      ((unsigned __int128)1 << k);

                /* Skip values that would not fit into seq_t. */
                if (v > seq_max) continue;

                /* Count 1‑digits in base 3. */
                unsigned __int128 temp = v;
                unsigned ones = 0;
                while (temp > 0) {
                    if ((temp % 3) == 1) ++ones;
                    temp /= 3;
                }
                if (ones == 3) {
                    if (idx < sizeof(terms) / sizeof(terms[0])) {
                        terms[idx++] = (seq_t)v;
                    }
                }
            }
        }
    }

    terms_len = idx;
    /* Ensure the list is sorted. */
    qsort(terms, terms_len, sizeof(seq_t), seq_cmp);
}

/* Return the n‑th term of the sequence (n≥1). */
seq_t a(idx_t n) {
    init_terms();
    if (n == 0 || (size_t)n > terms_len) return 0;  /* undefined, but safe */
    return terms[n - 1];
}
