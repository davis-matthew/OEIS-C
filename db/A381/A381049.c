/*
 * A381049  Triangle T(n,k) read by rows: where T(n,k) is the number of the
 *          k-th eliminated person in the variation of the Josephus
 *          elimination process for n people, where one person is skipped and
 *          two people are eliminated.
 * https://oeis.org/A381049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Simulates the Josephus variation where one person is
 * skipped and the next two are eliminated, producing the triangle
 * read by rows. Returns the n‑th element of that flattened triangle. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: generate elimination order for r people. */
static seq_t *simulate_row(idx_t r)
{
    /* Order will hold the elimination sequence; each value fits in seq_t. */
    seq_t *order = mem_new(seq_t, (size_t)r);
    if (!order) return NULL;

    /* alive[i] == true means person (i+1) is still present. */
    bool *alive = (bool *)malloc((size_t)r * sizeof(bool));
    if (!alive) { mem_free(order); return NULL; }

    for (idx_t i = 0; i < r; ++i) alive[i] = true;

    idx_t cur = 0;                 /* current pointer index (0‑based) */
    idx_t count_alive = r;         /* how many are still alive */
    idx_t out_idx = 0;             /* index in the order array */

    while (count_alive > 0)
    {
        /* Skip the person at cur and eliminate the next two alive ones. */
        for (int rem = 0; rem < 2 && count_alive > 0; ++rem)
        {
            idx_t next = (cur + 1) % r;
            while (!alive[next]) next = (next + 1) % r;  /* find next alive */

            alive[next] = false;                       /* eliminate */
            order[out_idx++] = (seq_t)(next + 1);       /* store 1‑based number */
            --count_alive;
        }

        if (count_alive == 0) break;

        /* Move the pointer to the next alive person after cur. */
        cur = (cur + 1) % r;
        while (!alive[cur]) cur = (cur + 1) % r;
    }

    free(alive);
    return order;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, but handled safely */

    /* Find row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }

    idx_t prev_cum = cum - r;          /* total terms before this row */
    idx_t pos_in_row = n - prev_cum - 1;  /* zero‑based index within the row */

    seq_t *row = simulate_row(r);      /* generate the elimination order */
    if (!row) return 0;                /* unlikely for small r, but safe */

    seq_t result = row[pos_in_row];
    mem_free(row);
    return result;
}
