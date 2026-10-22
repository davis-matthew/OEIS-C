/*
 * A347283  Square array read by antidiagonals upwards in which row n lists
 *          the parity of the 3x+1 sequence starting in n, with n >= 1 and k
 *          >= 0.
 * https://oeis.org/A347283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 4223, u16 n <= 9890, u32 n <= 9890, u64 n <= 9890,
 *            u128 n <= 9890, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = parity of the k-th element of the Collatz 3x+1 sequence
 * starting from n, where (n,k) is obtained by reading the square array
 * whose rows list parities of the Collatz sequence and flattening it by
 * antidiagonals upwards.  The row n contains the parities of the terms
 * of the sequence starting at n  (n ≥ 1, k ≥ 0).
 */
#include <stdint.h>

/* Helper: find the diagonal index d (starting from 0) that contains
 * overall position idx (1‑based).  The diagonal d has d+1 elements. */
static idx_t
diag_of_idx(idx_t idx)
{
    /* exponential search for an upper bound on d */
    idx_t low = 0;
    idx_t high = 1;
    while (((unsigned __int128)(high + 1) * (high + 2)) / 2 < idx)
        high <<= 1;

    /* binary search between low (exclusive) and high (inclusive) */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = ((unsigned __int128)(mid + 1) * (mid + 2)) / 2;
        if (val >= idx)
            high = mid;
        else
            low = mid + 1;
    }
    return low;   /* minimal d with cumulative count >= idx */
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    /* Determine the diagonal and offset within it */
    idx_t d = diag_of_idx(n);
    unsigned __int128 prev_cnt128 = ((unsigned __int128)d * (d + 1)) / 2;
    idx_t prev_cnt = (idx_t)prev_cnt128;          /* sum of elements before diagonal d */
    idx_t pos = n - prev_cnt - 1;                 /* 0‑based position in diagonal d */
    idx_t row = d + 1 - pos;                      /* starting number of Collatz sequence */
    idx_t k = pos;                                 /* step count */

    /* Compute k-th term of Collatz sequence starting at 'row' */
    seq_t cur = (seq_t)row;
    for (idx_t step = 0; step < k; ++step) {
        if ((cur & 1) == 0)
            cur >>= 1;        /* even case */
        else
            cur = cur * 3 + 1; /* odd case */
    }
    return cur & 1;   /* parity of the term */
}
