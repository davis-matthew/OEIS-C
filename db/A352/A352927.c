/*
 * A352927  Numbers whose digits are nonzero, consecutive, and all increasing
 *          or all decreasing.
 * https://oeis.org/A352927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: full, pass, 81 terms through n = 81 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A352927: numbers whose digits are nonzero, consecutive, and either
   all increasing or all decreasing.  The function generates all 81 such
   numbers, sorts them, and returns the n‑th (1‑based) value. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Comparator for qsort, works for both C and C++ with arbitrary-precision types */
static int cmp_seq_t_func(const void *p, const void *q)
{
    const seq_t *x = (const seq_t *)p;
    const seq_t *y = (const seq_t *)q;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Helper: generate the 81 monotonic consecutive‑digit numbers */
static void generate_sequence(seq_t *seq, size_t *count)
{
    size_t idx = 0;
    for (int i = 1; i <= 9; ++i) {
        for (int j = 1; j <= 9; ++j) {
            seq_t num = 0;
            if (i == j) {
                num = i;
            } else {
                int step = (j > i) ? 1 : -1;
                int d = i;
                while (1) {
                    num = num * 10 + d;
                    if (d == j) break;
                    d += step;
                }
            }
            seq[idx++] = num;
        }
    }
    *count = idx;
}

/* External-linkage function implementing the sequence */
seq_t a(idx_t n)
{
    static seq_t seq[81];
    static bool initialized = false;
    static size_t count = 0;

    if (!initialized) {
        generate_sequence(seq, &count);
        mem_qsort(seq, count, sizeof(seq_t), cmp_seq_t_func);
        initialized = true;
    }

    if (n == 0) return 0;                 /* undefined for offset 0 */
    idx_t idx = n - 1;
    if ((size_t)idx >= count) return 0;    /* out of range */
    return seq[idx];
}
