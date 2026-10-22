/*
 * A308825  Sum of the third largest parts of the partitions of n into 5
 *          parts.
 * https://oeis.org/A308825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A308825
    Sum of the third largest parts of the partitions of n into 5 parts.
    Formula: a(n) = Σ_{l=1}^{⌊n/5⌋} Σ_{k=l}^{⌊(n−l)/4⌋}
                 Σ_{j=k}^{⌊(n−k−l)/3⌋} Σ_{i=j}^{⌊(n−j−k−l)/2⌋} j
*/

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_free.h"

/* Helper to allocate an array of seq_t with given length */
static seq_t* seq_alloc(size_t len) {
    return (seq_t*)malloc(len * sizeof(seq_t));
}

/* Helper to free an array of seq_t */

seq_t a(idx_t n) {
    if (n < 5) return 0;

    idx_t mmax = n - 5;
    size_t len = (size_t)(mmax + 1);

    seq_t* count   = seq_alloc(len);
    seq_t* sum_a   = seq_alloc(len);
    seq_t* sum_b   = seq_alloc(len);
    seq_t* sum_c   = seq_alloc(len);

    /* Initialise arrays to zero */
    for (size_t i = 0; i < len; ++i) {
        count[i] = 0;
        sum_a[i] = 0;
        sum_b[i] = 0;
        sum_c[i] = 0;
    }
    count[0] = 1;

    /* Weight 5 – variable a */
    for (idx_t s = 5; s <= mmax; ++s) {
        seq_t prev_cnt = count[s - 5];
        if (prev_cnt != 0) {
            count[s] += prev_cnt;
            sum_a[s] += sum_a[s - 5] + prev_cnt;
            sum_b[s] += sum_b[s - 5];
            sum_c[s] += sum_c[s - 5];
        }
    }

    /* Weight 4 – variable b */
    for (idx_t s = 4; s <= mmax; ++s) {
        seq_t prev_cnt = count[s - 4];
        if (prev_cnt != 0) {
            count[s] += prev_cnt;
            sum_a[s] += sum_a[s - 4];
            sum_b[s] += sum_b[s - 4] + prev_cnt;
            sum_c[s] += sum_c[s - 4];
        }
    }

    /* Weight 3 – variable c */
    for (idx_t s = 3; s <= mmax; ++s) {
        seq_t prev_cnt = count[s - 3];
        if (prev_cnt != 0) {
            count[s] += prev_cnt;
            sum_a[s] += sum_a[s - 3];
            sum_b[s] += sum_b[s - 3];
            sum_c[s] += sum_c[s - 3] + prev_cnt;
        }
    }

    /* Weight 2 – variable d (no sum needed) */
    for (idx_t s = 2; s <= mmax; ++s) {
        seq_t prev_cnt = count[s - 2];
        if (prev_cnt != 0) {
            count[s] += prev_cnt;
            sum_a[s] += sum_a[s - 2];
            sum_b[s] += sum_b[s - 2];
            sum_c[s] += sum_c[s - 2];
        }
    }

    /* Weight 1 – variable e (no sum needed) */
    for (idx_t s = 1; s <= mmax; ++s) {
        seq_t prev_cnt = count[s - 1];
        if (prev_cnt != 0) {
            count[s] += prev_cnt;
            sum_a[s] += sum_a[s - 1];
            sum_b[s] += sum_b[s - 1];
            sum_c[s] += sum_c[s - 1];
        }
    }

    seq_t result = count[mmax] + sum_a[mmax] + sum_b[mmax] + sum_c[mmax];

    seq_free(count);
    seq_free(sum_a);
    seq_free(sum_b);
    seq_free(sum_c);

    return result;
}
