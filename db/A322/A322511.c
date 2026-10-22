/*
 * A322511  Nonnegative numbers whose digits can be formed by typing adjacent
 *          keys on a 123-456-789 keypad without repeating a digit and without
 *          changing direction.
 * https://oeis.org/A322511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all positive integers whose decimal digits
   can be typed by moving along a straight line on a 3×3 keypad
   (digits 1–9) without repeating a digit and without changing direction.
   The set is finite; we generate it once, sort it, remove duplicates,
   and return the (n‑1)th element (for n ≥ 1). */

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
#define CAST_CONST_SEQ_T(p) static_cast<const seq_t*>(p)
#define CAST_SEQ_T(p) static_cast<seq_t*>(p)
#else
#define CAST_CONST_SEQ_T(p) ((const seq_t*)(p))
#define CAST_SEQ_T(p) ((seq_t*)(p))
#endif

/* Compare two seq_t values for qsort. */
static int seq_cmp(const void *p1, const void *p2) {
    const seq_t *x = CAST_CONST_SEQ_T(p1);
    const seq_t *y = CAST_CONST_SEQ_T(p2);
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Storage for the generated sequence.  The number of elements is
   known to be at most 45, so an array of 64 bytes is plenty. */
static seq_t seq[64];
static size_t seq_len = 0;
static bool seq_initialized = false;

/* Build the sequence once. */
static void build_sequence(void) {
    if (seq_initialized) return;

    seq_t temp[200];
    size_t temp_len = 0;

    const int keys[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    /* Four allowed directions: up, down, left, right. */
    const int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int d = 0; d < 4; ++d) {
                int x = i, y = j;
                seq_t val = 0;
                /* walk until the edge of the keypad */
                while (x >= 0 && x < 3 && y >= 0 && y < 3) {
                    val = val * 10 + (seq_t)keys[x][y];
                    temp[temp_len++] = val;
                    x += dirs[d][0];
                    y += dirs[d][1];
                }
            }
        }
    }

    /* Sort and deduplicate */
    qsort(temp, temp_len, sizeof(seq_t), seq_cmp);

    seq_len = 0;
    for (size_t k = 0; k < temp_len; ++k) {
        if (k == 0 || temp[k] != temp[k - 1]) {
            seq[seq_len++] = temp[k];
        }
    }

    seq_initialized = true;
}

/* Public function required by the test harness. */
seq_t a(idx_t n) {
    build_sequence();
    if (n == 0) return 0;          /* offset is 1, so a(1) is first term */
    if ((idx_t)(n - 1) < (idx_t)seq_len)
        return seq[n - 1];
    return 0;  /* undefined for n beyond the length of the definition */
}
