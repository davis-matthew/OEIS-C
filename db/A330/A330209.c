/*
 * A330209  Nonzero coefficients of the polynomials (x + d/dx)^n x^2, in
 *          row-major order.
 * https://oeis.org/A330209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 45, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: The polynomial P_n(x) = (x + d/dx)^n x^2 satisfies
   P_0(x) = x^2,
   P_n(x) = x * P_{n-1}(x) + d/dx P_{n-1}(x)   (n ≥ 1).
   Its coefficients are generated in ascending degree order.
   Sequence a(k) lists all non‑zero coefficients of P_n(x) for
   n = 0, 1, 2, …, in row‑major order. */

#include <stddef.h>
#include "../utils/seq_realloc.h"

/* Cache of the sequence values. */
static seq_t *seq_cache = NULL;       /* dynamic array of a(n)   */
static idx_t seq_size = 0;            /* number of elements    */
static size_t seq_cap  = 0;           /* current capacity      */

/* Current row of coefficients (non‑zero ones only). */
static seq_t cur_row[5005];   /* max size for n ≤ 10000 */
static size_t cur_len  = 0;   /* number of coefficients in cur_row */
static idx_t  cur_n    = 0;   /* n of the row stored in cur_row */
static int    cur_base = 2;   /* lowest degree of cur_row
                               (2 for n = 0, 0 for even n>0, 1 for odd n) */

/* Ensure the cache contains at least the first 'need' terms. */
static void ensure_cache(idx_t need)
{
    /* Initialise on first use. */
    if (seq_cache == NULL) {
        /* start with a small capacity. */
        seq_cap = 64;
        seq_cache = seq_realloc(NULL, 0, seq_cap);
        /* n = 0: only the coefficient of x^2 is non‑zero. */
        cur_len  = 1;
        cur_row[0] = (seq_t)1;
        cur_n    = 0;
        cur_base = 2;
        seq_cache[0] = cur_row[0];
        seq_size = 1;
    }

    while ((idx_t)seq_size <= need) {
        /* compute the next row: n = cur_n + 1 */
        idx_t   next_n  = cur_n + 1;
        int     next_base = (next_n == 0) ? 2 : ((next_n & 1) ? 1 : 0);
        size_t  next_len = (next_n == 0) ? 1 : (size_t)(next_n / 2 + 2);

        /* zero initialise the next row array */
        seq_t next_row[5005];
        for (size_t i = 0; i < next_len; ++i)
            next_row[i] = 0;

        /* transfer contributions from the previous row */
        for (size_t j = 0; j < cur_len; ++j) {
            seq_t old_val = cur_row[j];
            int   old_deg = cur_base + (int)(2 * j);

            /* contribution from the "up" step: old_deg → old_deg + 1 */
            int   up_deg = old_deg + 1;
            size_t idx_up = (size_t)((up_deg - next_base) / 2);
            next_row[idx_up] += old_val;

            /* contribution from the "down" step: old_deg → old_deg - 1 */
            int   down_deg = old_deg - 1;
            if (down_deg >= next_base) {
                size_t idx_down = (size_t)((down_deg - next_base) / 2);
                next_row[idx_down] += (seq_t)old_deg * old_val;
            }
        }

        /* grow the cache if necessary */
        if (seq_size + next_len > seq_cap) {
            size_t new_cap = seq_cap;
            while (seq_size + next_len > new_cap)
                new_cap *= 2;
            seq_cache = seq_realloc(seq_cache, seq_cap, new_cap);
            seq_cap = new_cap;
        }

        /* append the new row's coefficients to the cache */
        for (size_t i = 0; i < next_len; ++i)
            seq_cache[seq_size++] = next_row[i];

        /* prepare for next iteration */
        cur_n    = next_n;
        cur_base = next_base;
        cur_len  = next_len;
        for (size_t i = 0; i < cur_len; ++i)
            cur_row[i] = next_row[i];
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[(size_t)n];
}
