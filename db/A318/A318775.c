/*
 * A318775  Triangle read by rows: T(0,0) = 1; T(n,k) = T(n-1,k) + 2 *
 *          T(n-5,k-1) for k = 0..floor(n/5); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A318775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A318775
 *
 * The OEIS entry defines a triangle T(n,k) with
 *
 *   T(0,0) = 1;
 *   T(n,k) = T(n-1,k) + 2*T(n-5,k-1)   for 0 ≤ k ≤ ⌊n/5⌋,
 *   T(n,k) = 0                           otherwise.
 *
 * The sequence a(n) is the n‑th element of this triangle when the rows
 * are concatenated in order.  The function below builds the triangle
 * lazily, caching all rows that have been generated so far.  A call to
 * a(n) guarantees that all terms up to n have been computed.
 */

#include <stdlib.h>   /* malloc, realloc, free, size_t */
#include <stdint.h>   /* uint64_t */

/* static cache for flattened triangle entries and row start indices */
static seq_t *terms      = NULL;   /* flattened triangle values */
static size_t terms_cap   = 0;     /* total capacity in terms */
static size_t terms_len   = 0;     /* number of terms stored */

static idx_t *row_start = NULL;   /* index into terms where each row begins */
static size_t row_cap   = 0;       /* total capacity in rows */
static size_t row_len   = 0;       /* number of rows stored */

/* enlarge the terms buffer if needed */
static void ensure_terms_capacity(size_t needed)
{
    if (needed > terms_cap) {
        size_t newcap = terms_cap ? terms_cap * 2 : 8;
        while (newcap < needed) newcap *= 2;
        terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
        terms_cap = newcap;
    }
}

/* enlarge the row_start buffer if needed */
static void ensure_row_capacity(size_t needed)
{
    if (needed > row_cap) {
        size_t newcap = row_cap ? row_cap * 2 : 8;
        while (newcap < needed) newcap *= 2;
        row_start = (idx_t *)realloc(row_start, newcap * sizeof(idx_t));
        row_cap = newcap;
    }
}

/* generate the triangle rows until the n‑th term is present in the cache */
static void generate_till(idx_t n)
{
    while (terms_len <= n) {
        /* current row number */
        idx_t cur_row = (idx_t)row_len;

        /* number of k values for this row = ⌊row/5⌋ + 1 */
        size_t len = (size_t)(1 + cur_row / 5);

        /* reserve space for this row */
        ensure_terms_capacity(terms_len + len);
        ensure_row_capacity(row_len + 1);

        /* record where this row starts */
        row_start[row_len] = (idx_t)terms_len;

        for (size_t k = 0; k < len; ++k) {
            seq_t val = 0;

            if (cur_row == 0 && k == 0) {
                val = 1;                     /* base case */
            } else {
                /* contribution from T(cur_row-1, k) if valid */
                if (cur_row > 0) {
                    size_t prev_len = 1 + (size_t)(cur_row - 1) / 5;
                    if (k < prev_len) {
                        val = terms[(size_t)row_start[row_len - 1] + k];
                    }
                }

                /* contribution from 2 * T(cur_row-5, k-1) if valid */
                if (cur_row >= 5 && k > 0) {
                    size_t prev5_len = 1 + (size_t)(cur_row - 5) / 5;
                    if (k - 1 < prev5_len) {
                        seq_t add = terms[(size_t)row_start[row_len - 5] + (k - 1)];
                        val += (seq_t)2 * add;
                    }
                }
            }

            terms[terms_len++] = val;
        }
        ++row_len;     /* move to next row */
    }
}

seq_t a(idx_t n)
{
    generate_till(n);
    return terms[(size_t)n];
}
