/*
 * A278827  Queen's moves in chess: possible difference between origin and
 *          destination square when the squares are numbered sequentially, row
 *          by row.
 * https://oeis.org/A278827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, fini, full
 * Formula:   generate all non‑zero integers d that can be written as ±k (1 ≤
 *            k ≤ 7), ±8k (1 ≤ k ≤ 7), ±7k (1 ≤ k ≤ 7), or ±9k (1 ≤ k ≤ 7).
 *            The values are sorted in ascending numeric order
 * Range:     i8 n <= 54, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t diff_vals[60];
static size_t diff_num = 0;
static bool diff_inited = false;

static void init_differences(void)
{
    seq_t tmp[70];
    size_t t = 0;

    /* horizontal moves: ±1 … ±7 */
    for (int k = 1; k <= 7; ++k) {
        tmp[t++] = (seq_t)k;
        tmp[t++] = -(seq_t)k;
    }
    /* vertical moves: ±8, ±16, …, ±56 */
    for (int k = 1; k <= 7; ++k) {
        tmp[t++] = (seq_t)(8 * k);
        tmp[t++] = -(seq_t)(8 * k);
    }
    /* diagonal 1 (±7, ±14, …, ±49) */
    for (int k = 1; k <= 7; ++k) {
        tmp[t++] = (seq_t)(7 * k);
        tmp[t++] = -(seq_t)(7 * k);
    }
    /* diagonal 2 (±9, ±18, …, ±63) */
    for (int k = 1; k <= 7; ++k) {
        tmp[t++] = (seq_t)(9 * k);
        tmp[t++] = -(seq_t)(9 * k);
    }

    /* insertion sort */
    for (size_t i = 1; i < t; ++i) {
        for (size_t j = i; j > 0 && tmp[j - 1] > tmp[j]; --j) {
            seq_t swap = tmp[j];
            tmp[j] = tmp[j - 1];
            tmp[j - 1] = swap;
        }
    }

    /* remove duplicates */
    size_t u = 0;
    for (size_t i = 0; i < t; ++i) {
        if (u == 0 || tmp[i] != tmp[u - 1]) {
            tmp[u++] = tmp[i];
        }
    }

    for (size_t i = 0; i < u; ++i) {
        diff_vals[i] = tmp[i];
    }
    diff_num = u;
    diff_inited = true;
}

seq_t a(idx_t n)
{
    if (!diff_inited) init_differences();

    /* a(n) is defined only for 1 ≤ n ≤ diff_num.  For out‑of‑range n
       we return 0 (undefined value). */
    if (n == 0 || n - 1 >= diff_num) return 0;
    return diff_vals[n - 1];
}
