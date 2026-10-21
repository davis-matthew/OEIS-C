/*
 * A294548  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + n - 1, where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + a(n‑2) + b(n‑1) + n - 1, a(0) = 1, a(1) = 2,
 *            b(0) = 3 a(n) and b(n) are increasing complementary sequences.
 *            b(n) is the least unused integer after the previous b
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static bool contains_a(const seq_t *a, size_t len, seq_t v) {
    for (size_t i = 0; i < len; ++i) {
        if (a[i] == v) return true;
        if (a[i] > v) break;         /* array sorted */
    }
    return false;
}

seq_t a(idx_t n) {
    /* static memoised arrays */
    static seq_t *seq = NULL;
    static idx_t cap = 0;          /* allocated capacity */
    static idx_t built = 0;        /* number of terms already built */
    static seq_t last_b = 0;       /* last computed b */

    /* initialise on first call */
    if (seq == NULL) {
        cap = 16;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;            /* allocation failure */
        seq[0] = (seq_t)1;                    /* a(0) */
        seq[1] = (seq_t)2;                    /* a(1) */
        built = 2;                            /* we have a(0) and a(1) */
        last_b = (seq_t)3;                    /* b(0) */
    }

    /* enlarge storage if necessary */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */
        seq = tmp;
        cap = new_cap;
    }

    /* if we already have the needed term, return it */
    if (n < built) return seq[n];

    /* build terms up to n */
    for (idx_t i = built; i <= n; ++i) {
        /* generate b(i-1) that will be used for a(i) */
        seq_t candidate = last_b + (seq_t)1;
        while (contains_a(seq, (size_t)i, candidate))
            candidate += (seq_t)1;
        last_b = candidate;           /* now b(i-1) */

        /* compute a(i) */
        seq_t term = seq[i-1] + seq[i-2] + last_b +
                     (seq_t)(i-1);     /* n-1 = i-1 */
        seq[i] = term;
    }
    built = n + 1;                    /* we have built up to n */
    return seq[n];
}
