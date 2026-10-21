/*
 * A276955  Square array A(row,col): A(row,1) = A273670(row-1), and for col >
 *          1, A(row,col) = A153880(A(row,col-1)); Dispersion of factorial
 *          base left shift A153880.
 * https://oeis.org/A276955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = A(row,col) where A(row,1) = A273670(row-1) and for col>1,
 *            A(row,col) = A153880(A(row,col-1)), i.e. shift the
 *            factorial‑base representation by one zero
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 78, u64 n <= 210,
 *            u128 n <= 595, big any n
 * Helpers:   shift_n, shift_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/shift_n.h"
#include "../utils/shift_one.h"

/* Allocate enough space for rows needed for typical n ≤ 100000 */
#define MAX_ROWS 10000

/* Storage for the first column of each row (1‑based indices).*/
static seq_t first_col[MAX_ROWS + 1];
static idx_t first_col_cnt = 0;     /* number of rows already filled */
static seq_t next_candidate = 1;    /* next integer to test for mex */

/* Check whether a candidate has already appeared in any earlier row. */
static bool is_used(seq_t cand, idx_t upto_row)
{
    for (idx_t r = 1; r < upto_row; ++r)
    {
        seq_t val = first_col[r];
        if (val == cand)
            return true;

        seq_t cur = val;
        while (true)
        {
            seq_t nxt = shift_one(cur);
            if (nxt == cand)
                return true;
            if (nxt > cand)
                break;
            cur = nxt;
        }
    }
    return false;
}

/* Generate rows up to target_row (1‑based). */
static void generate_rows(idx_t target_row)
{
    while (first_col_cnt < target_row)
    {
        /* Find the smallest unused integer for the next row. */
        while (is_used(next_candidate, first_col_cnt + 1))
            ++next_candidate;

        ++first_col_cnt;
        first_col[first_col_cnt] = next_candidate;
    }
}

/* The required function */
seq_t a(idx_t n)
{
    if (n == 0)            /* sequence starts at n=1 */
        return 0;

    /* Find diagonal s where T(s)=s*(s+1)/2 ≥ n */
    idx_t low = 1, high = 1;
    while ((high * (high + 1)) / 2 < n)
        high <<= 1;
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if ((mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    /* Coordinates of the n‑th term in the array */
    idx_t prev_tri = (s - 1) * s / 2;    /* T(s‑1) */
    idx_t row = n - prev_tri;            /* 1‑based row */
    idx_t col = s - row + 1;             /* 1‑based column */

    /* Make sure we have computed the first column up to this row. */
    generate_rows(row);

    /* The term is the (col‑1)-shift of the first column of this row. */
    seq_t base = first_col[row];
    return shift_n(base, col - 1);
}
