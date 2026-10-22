/*
 * A375300  Triangle T(n,k) read by rows that has the A001710(n)-th
 *          lexicographic permutation of [1,...,n] as row n.
 * https://oeis.org/A375300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = element at position t of the (k! / 2)-th lexicographic
 *            permutation of [1..k], where k is the row containing index n.
 *            The row index k is the unique integer satisfying k(k-1)/2 < n ≤
 *            k(k+1)/2; t = n - k(k-1)/2 - 1 (zero–based)
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 78, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

/* Finds the row index k such that k(k-1)/2 < N ≤ k(k+1)/2. */
static idx_t find_row(idx_t N)
{
    idx_t lo = 1, hi = 1;
    /* Grow hi until it satisfies the condition. */
    while ((unsigned __int128)hi * (hi + 1) / 2 < N) hi <<= 1;
    lo = hi >> 1;

    /* Binary search between lo and hi. */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= N)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Sufficiently large for the expected maximum row. */
#define MAX_ROW 4000
static seq_t fact_table[MAX_ROW + 1];

seq_t a(idx_t N)
{
    if (N < 1) return 0;                     /* safety, not part of OEIS offset */
    idx_t row = find_row(N);

    /* Compute zero‑based index within the row. */
    unsigned __int128 prev_sum = (unsigned __int128)(row - 1) * row / 2;
    idx_t t = N - prev_sum - 1;              /* zero‑based position in row */

    if (row == 1) return 1;                  /* first row is just [1] */

    /* Compute factorials up to 'row'. */
    fact_table[0] = 1;
    for (idx_t i = 1; i <= row; ++i)
        fact_table[i] = fact_table[i - 1] * (seq_t)i;

    /* Rank of the permutation: (row! / 2) – 1 (zero‑based). */
    seq_t rank = fact_table[row] / 2;
    rank -= 1;

    /* Available numbers for permutation construction. */
    idx_t avail[MAX_ROW];
    for (idx_t i = 0; i < row; ++i)
        avail[i] = i + 1;
    idx_t avail_len = row;

    /* Generate the permutation up to the needed position t. */
    for (idx_t pos = 0; pos < row; ++pos) {
        seq_t f       = fact_table[row - 1 - pos];
        seq_t idx_big = rank / f;           /* ≤ avail_len-1 */
        idx_t idx    = (idx_t)idx_big;

        if (pos == t)
            return (seq_t)avail[idx];

        /* Remove chosen element from avail. */
        for (idx_t j = idx; j + 1 < avail_len; ++j)
            avail[j] = avail[j + 1];
        --avail_len;
        rank = rank % f;
    }

    return 0;   /* Should never be reached */
}
