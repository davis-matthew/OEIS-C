/*
 * A380202  Number of card moves to deal n cards using the SpellUnder-Down
 *          dealing.
 * https://oeis.org/A380202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = A067278(n) + n where A067278(n) = sum_{k=1}^n
 *            word_length(k)
 * Range:     u8 n <= 31, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   word_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/word_len.h"

/* Static cache of the cumulative sums of word lengths.
   sum_arr[0] is always 0; sum_arr[n] holds the value of A067278(n).
   The array is grown on demand. */
static seq_t *sum_arr   = NULL;
static idx_t  capacity  = 0;
static idx_t  max_computed = 0;

/* Ensure that sum_arr has space for indices up to 'need'. */
static void
grow_sum_arr(idx_t need)
{
    if (capacity > need)
        return;
    idx_t new_cap = capacity ? capacity * 2 : 1;
    while (new_cap <= need)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(sum_arr, new_cap * sizeof(seq_t));
    if (!new_arr)   /* allocation failure: leave existing array unchanged */
        return;
    sum_arr = new_arr;
    capacity = new_cap;
    /* Newly allocated tail is unspecified; we will overwrite values
       before using them, so no need to zero them. */
    /* Ensure the base case 0 is preserved */
    sum_arr[0] = (seq_t)0;
}

/* Main function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* offset is 1; this is never requested normally */

    grow_sum_arr(n);

    /* Compute cumulative sums up to n if not already done. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t len = word_len(i);
        sum_arr[i] = sum_arr[i - 1] + len;
    }
    max_computed = n;

    /* a(n) = A067278(n) + n */
    return sum_arr[n] + (seq_t)n;
}
