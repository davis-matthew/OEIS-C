/*
 * A381587  a(1) = 1; thereafter the sequence is extended by iteratively
 *          appending the run length transform of the reverse of the sequence
 *          thus far.
 * https://oeis.org/A381587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9259, u16 n <= 9259, u32 n <= 9259, u64 n <= 9259,
 *            u128 n <= 9259, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A381587: a(1)=1; thereafter iteratively append the run‑length transform 
 * of the reverse of the sequence so far.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for the sequence and its current size */
static seq_t *seq = NULL;          /* array of values */
static idx_t seq_len = 0;          /* number of values generated */
static size_t seq_cap = 0;         /* capacity of the array */

/* Ensure the array has at least 'needed' slots */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = tmp;
    seq_cap = new_cap;
}

/* Generate the sequence up to at least position 'n' (1‑based) */
static void generate_up_to(idx_t n)
{
    if (seq == NULL) {
        /* Initial allocation and base term */
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        seq[seq_len++] = (seq_t)1;
    }

    while (seq_len < n) {
        /* Preserve old length to avoid reading newly appended values */
        idx_t old_len = seq_len;

        /* Run‑length transform of the reverse of the current sequence
           is computed by scanning from the last element backward. */
        seq_t prev = seq[old_len - 1];
        seq_t run = 1;

        for (idx_t i = old_len - 1; i > 0; --i) {
            seq_t cur = seq[i - 1];
            if (cur == prev) {
                ++run;
            } else {
                ensure_capacity(seq_len + 1);
                seq[seq_len++] = run;
                prev = cur;
                run  = 1;
            }
        }
        /* Append the final run length */
        ensure_capacity(seq_len + 1);
        seq[seq_len++] = run;
    }
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, but return 0 for safety */
    generate_up_to(n);
    return seq[n - 1];
}
