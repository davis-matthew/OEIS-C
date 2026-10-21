/*
 * A290608  Number of maximal independent vertex sets (and minimal vertex
 *          covers) in the n-Moebius ladder graph.
 * https://oeis.org/A290608
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n) - 2*cos(2*n*Pi/3) Recurrence: a(n) = a(n-2) +
 *            2*a(n-3) + a(n-4) Initial values: a(3)=2, a(4)=8, a(5)=12,
 *            a(6)=16
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"

static seq_t *memo = NULL;
static size_t memo_len = 0;

static void ensure_memo(idx_t n)
{
    if (memo_len > n)
        return;

    size_t new_len = (size_t)n + 1;
    seq_t *new_arr = seq_alloc(new_len);
    if (!new_arr)
        exit(1);

    /* copy existing values */
    for (size_t i = 0; i < memo_len; ++i)
        new_arr[i] = memo[i];

    /* replace the old array */
    memo = new_arr;

    size_t old_len = memo_len;
    memo_len = new_len;

    /* initialise base cases when they become available */
    if (new_len > 3 && old_len <= 3)
        memo[3] = (seq_t)2;
    if (new_len > 4 && old_len <= 4)
        memo[4] = (seq_t)8;
    if (new_len > 5 && old_len <= 5)
        memo[5] = (seq_t)12;
    if (new_len > 6 && old_len <= 6)
        memo[6] = (seq_t)16;

    /* compute recurrence for new entries */
    size_t start = (old_len > 7) ? old_len : 7;
    for (size_t i = start; i < new_len; ++i) {
        memo[i] = memo[i - 2] + memo[i - 3] * (seq_t)2 + memo[i - 4];
    }
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;   /* undefined below offset */

    ensure_memo(n);
    return memo[(size_t)n];
}
