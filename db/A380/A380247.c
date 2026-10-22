/*
 * A380247  Triangle read by rows: T(n,k) is the number of the k-th eliminated
 *          person in the variation of the Josephus elimination process for n
 *          people, where the number of people skipped correspond to the
 *          number of letters in the next number in English alphabet.
 * https://oeis.org/A380247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, tabl
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   wordlen_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by simulation of the Josephus variation with skip lengths equal to the number of letters in the English words of successive natural numbers. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/wordlen_uint64.h"

/* seq_t and idx_t are defined by the build system. */
extern idx_t _; /* placeholder only */

/* Global buffer for the concatenated output sequence */
static seq_t *global_terms = NULL;
static size_t global_cap   = 0;
static idx_t  global_size  = 0;
static idx_t  next_row     = 1;            /* next row to be generated */

/* Append a value to the global sequence buffer. */
static void global_append(seq_t val)
{
    if (global_size == global_cap) {
        size_t newcap = (global_cap == 0) ? 16 : global_cap * 2;
        global_terms = (seq_t*)realloc(global_terms, newcap * sizeof(seq_t));
        global_cap   = newcap;
    }
    global_terms[global_size++] = val;
}

/* Compute the elimination order for a circle of size n and append to the global buffer. */
static void compute_row(idx_t n)
{
    int *next  = (int*)malloc((n + 1) * sizeof(int));
    int *prev  = (int*)malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= (int)n; ++i) {
        next[i] = (i == (int)n) ? 1 : (i + 1);
        prev[i] = (i == 1) ? (int)n : (i - 1);
    }

    int cur = 1;  /* starting pointer */

    for (idx_t step = 1; step <= n; ++step) {
        uint64_t skip = wordlen_uint64(step);
        for (uint64_t s = 0; s < skip; ++s) {
            cur = next[cur];
        }

        int elim = cur;
        global_append((seq_t)elim);

        /* Remove elim from the circle */
        int prev_elim = prev[elim];
        int next_elim = next[elim];
        next[prev_elim] = next_elim;
        prev[next_elim] = prev_elim;

        cur = next_elim; /* move to the next survivor */
    }

    free(next);
    free(prev);
}

/* Required function definition */
seq_t a(idx_t n)
{
    while (global_size < n) {
        compute_row(next_row);
        ++next_row;
    }
    return global_terms[n - 1];
}
