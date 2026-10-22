/*
 * A394676  a(1) = 1, a(n+1) as the smallest positive integer greater than
 *          a(n) that avoids violating the condition a(k) + a(k+1) + a(k+2) +
 *          a(k+3) is not term.
 * https://oeis.org/A394676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   contains_sorted
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A394676:  a(1)=1;  a(n+1) is the smallest integer > a(n)
   such that no sum of any four consecutive terms equals any term
   of the sequence.  The code builds the sequence on demand. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/contains_sorted.h"

/* Dynamic array for the sequence values */
static seq_t *seq   = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Dynamic array for the sums of each 4‑term window (strictly increasing) */
static seq_t *sums   = NULL;
static size_t sums_len = 0;
static size_t sums_cap = 0;

/* Linear search on an array of seq_t values. */
static bool contains_seq(const seq_t *arr, size_t len, seq_t value)
{
    for (size_t i = 0; i < len; ++i)
        if (arr[i] == value) return true;
    return false;
}

/* Ensure a dynamic array has space for at least 'needed' elements,
   copying 'used' initialized elements. */
static void ensure_capacity(size_t used, size_t needed,
                            size_t *cap, seq_t **arr)
{
    if (needed <= *cap) return;
    size_t new_cap = *cap ? *cap : 1;
    while (new_cap < needed) new_cap <<= 1;
#if defined(__cplusplus)
    seq_t *new_arr = new seq_t[new_cap];
    for (size_t i = 0; i < used; ++i) new_arr[i] = (*arr)[i];
    delete[] *arr;
    *arr = new_arr;
#else
    seq_t *new_arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    *arr = new_arr;
#endif
    *cap = new_cap;
}

/* Generate the sequence up to index 'upto' (1‑based). */
static void generate_to(idx_t upto)
{
    while (seq_len < (size_t)upto) {
        /* Allocate space for the new term. */
        ensure_capacity(seq_len, seq_len + 1, &seq_cap, &seq);

        /* Candidate starts just after the last term. */
        seq_t last = seq_len ? seq[seq_len - 1] : 0;
        seq_t cand  = last + (seq_t)1;

        while (true) {
            /* Candidate must not equal any previously seen 4‑term sum. */
            if (sums_len && contains_sorted(sums, sums_len, cand)) {
                ++cand;
                continue;
            }

            /* If we have at least three previous terms, the sum
               that would include the candidate as the fourth term
               must not already exist in the sequence. */
            if (seq_len >= 3) {
                seq_t new_sum = seq[seq_len - 3] + seq[seq_len - 2] +
                                seq[seq_len - 1] + cand;
                if (contains_seq(seq, seq_len, new_sum)) {
                    ++cand;
                    continue;
                }
            }
            /* All conditions satisfied. */
            break;
        }

        /* Append the chosen candidate. */
        seq[seq_len++] = cand;

        /* After adding the new element we now have a new 4‑term
           window.  Append its sum. */
        if (seq_len >= 4) {
            ensure_capacity(sums_len, sums_len + 1, &sums_cap, &sums);
            sums[sums_len++] = seq[seq_len - 4] + seq[seq_len - 3] +
                               seq[seq_len - 2] + seq[seq_len - 1];
        }
    }
}

/* Public entry point required by the build. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n<1 */
    generate_to(n);
    return seq[(size_t)(n - (idx_t)1)];
}
