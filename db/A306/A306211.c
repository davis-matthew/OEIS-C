/*
 * A306211  Concatenation of the current sequence with the lengths of the runs
 *          in the sequence, with a(1) = 1.
 * https://oeis.org/A306211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) is the nth term of the sequence defined by starting with
 *            [1] and repeatedly appending the list of run lengths of the
 *            current sequence
 * Range:     u8 n <= 9489, u16 n <= 9489, u32 n <= 9489, u64 n <= 9489,
 *            u128 n <= 9489, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *seq = NULL;      /* storage for the sequence values      */
static size_t seq_len = 0;     /* how many values are stored so far    */
static size_t seq_cap = 0;     /* current allocation capacity          */

/* make sure the sequence array can hold at least `need` elements */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = tmp;
    seq_cap = new_cap;
}

/* generate the next block of run‑lengths and append it to the sequence */
static void generate_next(void)
{
    if (seq_len == 0) return;  /* should never happen */

    size_t run_cnt = 0;
    seq_t *runs = (seq_t *)malloc(seq_len * sizeof(seq_t));
    size_t i = 0;
    size_t run_len = 1;

    for (; i + 1 < seq_len; ++i) {
        if (seq[i] == seq[i + 1]) {
            ++run_len;
        } else {
            runs[run_cnt++] = (seq_t)run_len;
            run_len = 1;
        }
    }
    runs[run_cnt++] = (seq_t)run_len;  /* last run */

    ensure_capacity(seq_len + run_cnt);
    for (size_t j = 0; j < run_cnt; ++j) {
        seq[seq_len + j] = runs[j];
    }
    seq_len += run_cnt;
    free(runs);
}

/* return the n-th term (1‑based) of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for 0, but the function will never be called with 0 */

    /* initialise on first call */
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 1;
        seq[0] = (seq_t)1;   /* a(1) = 1 */
    }

    while (seq_len < (size_t)n) {
        generate_next();
    }

    return seq[(size_t)n - 1];
}
