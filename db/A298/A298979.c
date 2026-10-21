/*
 * A298979  Lexicographic first sequence of positive integers such that
 *          a(n)*a(n+1) has a digit 9, and no term occurs twice.
 * https://oeis.org/A298979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographic first permutation of positive integers such that
   a(n)*a(n+1) contains digit 9 (OEIS A298979). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_frequencies.h"

static seq_t *seq = NULL;          /* array of sequence values          */
static size_t seq_len = 0;         /* number of terms computed          */
static size_t seq_cap = 0;         /* current capacity of seq           */

static bool *used = NULL;         /* used[i] == true if i already appears  */
static size_t used_cap = 0;        /* current capacity of used array      */

/* Ensure seq has at least new_cap slots.  */
static void grow_seq(size_t new_cap)
{
    if (new_cap <= seq_cap)
        return;

    if (seq_cap == 0)
        seq_cap = 64;
    else
        seq_cap *= 2;
    while (seq_cap < new_cap)
        seq_cap <<= 1;

    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
}

/* Ensure used has at least new_cap slots.  */
static void grow_used(size_t new_cap)
{
    if (new_cap <= used_cap)
        return;

    if (used_cap == 0)
        used_cap = 128;
    else
        used_cap <<= 1;
    while (used_cap < new_cap)
        used_cap <<= 1;

    used = (bool *)realloc(used, used_cap * sizeof(bool));
    for (size_t i = used_cap / 2; i < used_cap; ++i)
        used[i] = false;
}

/* Return true if x contains a digit 9.  */
static bool has_digit9(seq_t x)
{
    unsigned int freq[10] = {0};
    digit_frequencies(x, freq);
    return freq[9] != 0;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined for n < 1 */

    /* Initialise storage on first call.  */
    if (!seq) {
        grow_seq(1);
        seq[0] = (seq_t)1;
        seq_len = 1;

        grow_used(2);
        used[1] = true;
    }

    /* Generate terms until we reach the required index.  */
    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        seq_t candidate = 1;          /* search from 1 upwards */

        while (true) {
            size_t idx = (size_t)candidate;
            grow_used(idx + 1);
            if (!used[idx]) {
                seq_t prod = prev * candidate;
                if (has_digit9(prod)) {
                    /* Accept candidate.  */
                    if (seq_len >= seq_cap)
                        grow_seq(seq_len + 1);

                    seq[seq_len] = candidate;
                    used[idx] = true;
                    ++seq_len;
                    break;
                }
            }
            candidate += 1;           /* cannot use ++ on seq_t */
        }
    }

    return seq[n - 1];
}
