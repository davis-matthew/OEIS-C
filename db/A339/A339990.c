/*
 * A339990  Sums of two odd cubes.
 * https://oeis.org/A339990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th smallest integer that can be written as * a sum
 *            of two odd cubes. The function precomputes all sums of * two odd
 *            cubes up to a fixed bound, sorts them, deduplicates, * and
 *            stores the sequence in a static array. Subsequent calls * simply
 *            index into that array
 * Range:     u8 none, u16 n <= 1, u32 n <= 26, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *sequence = NULL;          /* Array of summed values */
static size_t seq_len = 0;               /* Number of distinct sums */
static int initialized = 0;             /* Whether the sequence has been built */

/* Comparator for qsort – works for all integer‑like seq_t types. */
static int cmp_seq(const void *a, const void *b) {
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Build the sequence once.  The bound (odd_count = 2000) guarantees
 * more than 10 000 terms, which covers the expected range of n. */
static void initialize_sequence(void) {
    if (initialized) return;

    /* Number of odd integers to consider (1,3,5,…). */
    const idx_t odd_count = 2000;
    const idx_t pair_cnt   = odd_count * (odd_count + 1) / 2;

    /* Compute all odd cubes. */
    seq_t *cubes = (seq_t *)malloc(odd_count * sizeof(seq_t));
    if (!cubes) return;  /* Allocation failed – never use sequence. */
    idx_t odd = 1;
    for (idx_t i = 0; i < odd_count; ++i) {
        seq_t v = (seq_t)odd * odd * odd;
        cubes[i] = v;
        odd += 2;
    }

    /* Generate all unordered sums x^3 + y^3 (x <= y). */
    seq_t *sums = (seq_t *)malloc(pair_cnt * sizeof(seq_t));
    if (!sums) { free(cubes); return; }
    size_t idx = 0;
    for (idx_t i = 0; i < odd_count; ++i) {
        for (idx_t j = i; j < odd_count; ++j) {
            sums[idx++] = cubes[i] + cubes[j];
        }
    }
    free(cubes);

    /* Sort the sums. */
    qsort(sums, pair_cnt, sizeof(seq_t), cmp_seq);

    /* Deduplicate into the final sequence array. */
    seq_len = 0;
    if (pair_cnt > 0) {
        sequence = (seq_t *)malloc(pair_cnt * sizeof(seq_t));
        if (!sequence) {
            free(sums);
            return;
        }
        seq_t prev = sums[0];
        sequence[seq_len++] = prev;
        for (size_t k = 1; k < pair_cnt; ++k) {
            if (sums[k] != prev) {
                prev = sums[k];
                sequence[seq_len++] = prev;
            }
        }
    }
    free(sums);
    initialized = 1;
}

/* External function required by the build system. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1 */
    if (!initialized) initialize_sequence();
    if (n - 1 >= seq_len) return 0;  /* out of bounds – should not happen */
    return sequence[n - 1];
}
