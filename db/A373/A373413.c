/*
 * A373413  Sum of the n-th maximal run of squarefree numbers.
 * https://oeis.org/A373413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the sum of the numbers in the n-th maximal run of squarefree integers.
   A maximal run is a consecutive block of squarefree numbers that cannot be
   extended on either side. The function searches runs on demand, caches the
   sums in a static array, and returns the n-th sum (a(1) is the first such sum). */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_squarefree.h"

/* static state between calls */
static seq_t *runs = NULL;      /* array of sums of discovered squarefree runs */
static size_t runs_cap = 0;     /* allocated capacity of 'runs' */
static idx_t runs_count = 0;    /* number of runs stored */
static idx_t cur_number = 1;    /* next natural number to test */

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety guard; offset is 1 */

    /* Generate runs until we have at least n of them */
    while (runs_count < n) {
        /* Find start of the next maximal run: skip non‑squarefree numbers */
        while (!is_squarefree((seq_t)cur_number)) {
            ++cur_number;
        }

        seq_t start = (seq_t)cur_number;
        idx_t len = 1;
        ++cur_number;

        /* Continue while numbers remain squarefree */
        while (is_squarefree((seq_t)cur_number)) {
            ++len;
            ++cur_number;
        }

        /* Sum of consecutive integers from start to start+len-1 */
        seq_t last = start + (seq_t)(len - 1);
        seq_t run_sum = (seq_t)len * (start + last) / 2;

        /* Ensure capacity to store the new run sum */
        if (runs_count == runs_cap) {
            size_t new_cap = (runs_cap == 0) ? 32 : runs_cap * 2;
            seq_t *tmp = (seq_t *)realloc(runs, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* allocation failed */
            runs = tmp;
            runs_cap = new_cap;
        }
        runs[runs_count++] = run_sum;
    }

    return runs[n - 1];
}
