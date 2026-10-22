/*
 * A379399  Numbers that can be written in exactly four different ways as a
 *          sum of at most nine positive third powers.
 * https://oeis.org/A379399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the numbers that can be written in exactly four different ways as a sum
   of at most nine positive third powers (OEIS A379399). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"

#define MAX_SUM 3087            /* 9 × 7³ = 3087, the largest possible sum */

static const seq_t cubes[7] = {
    (seq_t)1,  (seq_t)8,   (seq_t)27,
    (seq_t)64, (seq_t)125, (seq_t)216,
    (seq_t)343
};

static seq_t counts[MAX_SUM + 1] = { 0 };   /* representation counter */

static seq_t *values        = NULL;         /* cached sequence values */
static idx_t  values_len    = 0;
static bool   initialized   = false;

/* Recursively enumerate unordered multisets of cubes with size ≤ 9 and
   increment counts for each resulting sum. */
static void rec(int pos, int used, seq_t sum)
{
    for (int i = pos; i < 7; ++i) {
        seq_t ns = sum + cubes[i];
        if (used + 1 <= 9) {
            counts[(int)ns] += (seq_t)1;
            rec(i, used + 1, ns);
        }
    }
}

/* Build the cached array of numbers with exactly four representations. */
static void init_seq(void)
{
    rec(0, 0, (seq_t)0);

    idx_t total = 0;
    for (int i = 1; i <= MAX_SUM; ++i) {
        if (counts[i] == (seq_t)4) {
            ++total;
        }
    }

    values = seq_alloc(total);
    idx_t idx = 0;
    for (int i = 1; i <= MAX_SUM; ++i) {
        if (counts[i] == (seq_t)4) {
            values[idx++] = (seq_t)i;
        }
    }
    values_len   = total;
    initialized = true;
}

/* OEIS entry function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; caller should not ask n=0 */

    if (!initialized) init_seq();

    if (n - 1 < values_len) return values[n - 1];
    return (seq_t)0;                /* beyond the known sequence */
}
