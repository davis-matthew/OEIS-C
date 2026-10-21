/*
 * A242264  Numbers that can be written as a sum of numbers using all nonzero
 *          decimal digits in descending order and also all nonzero decimal
 *          digits in ascending order.
 * https://oeis.org/A242264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A242264: Numbers that can be written as a sum of numbers
   using all nonzero decimal digits in descending order
   and also all nonzero decimal digits in ascending order.
   The function enumerates the 256 partitions of the nine digits
   for each order, computes the sums, takes their intersection,
   and returns the nth smallest element (n≥1). */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper comparator for qsort on seq_t */
static int compare_seq_t(const void *a, const void *b) {
    seq_t v1 = *(const seq_t *)a;
    seq_t v2 = *(const seq_t *)b;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* Static cache for memoisation */
static bool ready = false;
static seq_t terms[256];
static size_t term_count = 0;

/* Compute all terms up to the maximum (58) once */
static void compute_terms(void) {
    const unsigned char digits_desc[9] = {9,8,7,6,5,4,3,2,1};
    const unsigned char digits_asc[9]  = {1,2,3,4,5,6,7,8,9};

    seq_t sums_desc[256], sums_asc[256];
    size_t cnt_desc = 0, cnt_asc = 0;

    /* Partition digits in descending order */
    for (unsigned int mask = 0; mask < (1u << 8); ++mask) {
        seq_t sum = 0, cur = 0;
        for (int i = 0; i < 9; ++i) {
            cur = cur * 10 + digits_desc[i];
            if (i == 8 || (mask & (1u << i))) {
                sum += cur;
                cur = 0;
            }
        }
        sums_desc[cnt_desc++] = sum;
    }

    /* Partition digits in ascending order */
    for (unsigned int mask = 0; mask < (1u << 8); ++mask) {
        seq_t sum = 0, cur = 0;
        for (int i = 0; i < 9; ++i) {
            cur = cur * 10 + digits_asc[i];
            if (i == 8 || (mask & (1u << i))) {
                sum += cur;
                cur = 0;
            }
        }
        sums_asc[cnt_asc++] = sum;
    }

    /* Sort both lists */
    qsort(sums_desc, cnt_desc, sizeof(seq_t), compare_seq_t);
    qsort(sums_asc,  cnt_asc,  sizeof(seq_t), compare_seq_t);

    /* Merge to get the intersection, removing duplicates */
    size_t i = 0, j = 0;
    term_count = 0;
    while (i < cnt_desc && j < cnt_asc) {
        if (sums_desc[i] == sums_asc[j]) {
            if (term_count == 0 || terms[term_count - 1] != sums_desc[i]) {
                terms[term_count++] = sums_desc[i];
            }
            ++i; ++j;
        } else if (sums_desc[i] < sums_asc[j]) {
            ++i;
        } else {
            ++j;
        }
    }

    ready = true;
}

seq_t a(idx_t n) {
    if (!ready) compute_terms();

    if (n == 0 || n > term_count) return 0;
    /* idx_t starts at 1; array index is n-1 */
    return terms[n - 1];
}
