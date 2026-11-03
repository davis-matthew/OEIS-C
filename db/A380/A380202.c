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
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Compute the number of letters in the English name of a positive
   integer x (1 <= x < 10000).  The result is a small number that
   fits in any unsigned integer type. */
static seq_t word_len(idx_t x)
{
    seq_t len = 0;
    static const int unit_len[10] = {0, 3, 3, 5, 4, 4, 3, 5, 5, 4};
    static const int teen_len[10] = {3, 6, 6, 8, 8, 7, 7, 9, 8, 8};
    static const int tens_len[10] = {0, 0, 6, 6, 5, 5, 5, 7, 6, 6};

    const seq_t len_hundred  = 7;  /* "hundred" */
    const seq_t len_thousand = 8;  /* "thousand" */
    const seq_t len_and      = 3;  /* "and" */

    if (x >= 1000) {
        idx_t thousands = x / 1000;     /* 1 .. 9 for numbers < 10000 */
        len += unit_len[thousands] + len_thousand;
        x  %= 1000;
    }
    if (x >= 100) {
        idx_t hundreds = x / 100;
        len += unit_len[hundreds] + len_hundred;
        x  %= 100;
        if (x > 0) len += len_and;
    }
    if (x >= 20) {
        idx_t tens = x / 10;
        len += tens_len[tens];
        x  %= 10;
    }
    if (x >= 10) {
        len += teen_len[x - 10];
        x = 0;
    }
    if (x > 0) {
        len += unit_len[x];
    }
    return len;
}

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
    seq_t *new_arr = mem_resize(seq_t, sum_arr, new_cap);
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
