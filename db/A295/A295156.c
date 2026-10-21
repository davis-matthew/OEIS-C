/*
 * A295156  Numbers that have exactly eight representations as a sum of five
 *          nonnegative squares.
 * https://oeis.org/A295156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A295156: numbers that have exactly eight representations as a sum
   of five nonnegative squares (unordered). This implementation
   performs a brute‑force enumeration of all unordered 5‑tuples of squares
   up to the proven limit 5408, counts the number of representations for
   each integer, and stores those with exactly eight representations. */

#include <stdbool.h>
#include <stdlib.h>

/* Proven upper bound for the sequence (see OEIS comments). */
#define LIMIT 5408
/* floor(sqrt(LIMIT)) = 73. */
#define MAX_R 73

/* Global storage for the sequence terms. */
static seq_t *seq_arr    = NULL;   /* array of numbers with 8 reps */
static idx_t  seq_cnt    = 0;      /* number of such numbers */
static bool   seq_built  = false;  /* have we built the table? */

/* Build the sequence once. */
static void build_sequence(void)
{
    /* Count array: number of unordered representations for each n. */
    static unsigned count[LIMIT + 1] = {0};
    idx_t sq[MAX_R + 1];

    /* Pre‑compute squares. */
    for (idx_t i = 0; i <= MAX_R; ++i)
        sq[i] = i * i;

    /* Enumerate all non‑decreasing 5‑tuples (a,b,c,d,e). */
    for (idx_t a = 0; a <= MAX_R; ++a) {
        idx_t s2a = sq[a];
        for (idx_t b = a; b <= MAX_R; ++b) {
            idx_t s2ab = s2a + sq[b];
            if (s2ab > LIMIT) break;
            for (idx_t c = b; c <= MAX_R; ++c) {
                idx_t s3 = s2ab + sq[c];
                if (s3 > LIMIT) break;
                for (idx_t d = c; d <= MAX_R; ++d) {
                    idx_t s4 = s3 + sq[d];
                    if (s4 > LIMIT) break;
                    for (idx_t e = d; e <= MAX_R; ++e) {
                        idx_t s5 = s4 + sq[e];
                        if (s5 > LIMIT) break;
                        ++count[s5];
                    }
                }
            }
        }
    }

    /* Count how many n have exactly 8 representations. */
    seq_cnt = 0;
    for (idx_t n = 0; n <= LIMIT; ++n)
        if (count[n] == 8)
            ++seq_cnt;

    /* Allocate array and store the numbers. */
    if (seq_cnt > 0) {
        seq_arr = (seq_t *)malloc(sizeof(seq_t) * seq_cnt);
        if (seq_arr) {
            idx_t pos = 0;
            for (idx_t n = 0; n <= LIMIT; ++n)
                if (count[n] == 8)
                    seq_arr[pos++] = (seq_t)n;
        }
    }
    seq_built = true;
}

/* OEIS sequence function. */
seq_t a(idx_t n)
{
    if (!seq_built)
        build_sequence();

    if (n == 0 || n > seq_cnt || !seq_arr)
        return (seq_t)0;  /* undefined for this sequence */

    return seq_arr[n - 1];
}
