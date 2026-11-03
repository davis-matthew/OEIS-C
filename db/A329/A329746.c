/*
 * A329746  Triangle read by rows where T(n,k) is the number of integer
 *          partitions of n > 0 with runs-resistance k, 0 <= k <= n - 1.
 * https://oeis.org/A329746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 213, u16 n <= 1133, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Validated: standard, pass, 94 terms through n = 1181 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A329746: triangle of runs‑resistance counts for integer partitions.
   The sequence is read row‑by‑row:
   a(1)=T(1,0), a(2)=T(2,0), a(3)=T(2,1), a(4)=T(3,0), etc.
   This implementation builds triangle rows up to the requested
   index by enumerating all integer partitions of n and counting the
   number of steps required to reach a singleton via run‑lengths. */

#include <stdlib.h>
#include "../utils/mem.h"

/*  Compute the runs‑resistance of a partition given as a flat array.
   parts[0..len-1] contain the parts in non‑increasing order.
   The function works purely on int arrays; the resulting count fits
   easily in an int because the number of steps is < log₂(len)+1.       */
static int resistance(const int parts[], int len)
{
    /* Two temporary target arrays – sized generously. */
    /* The max number of parts for n <= 200 is 200; 200 suffices. */
    int cur[200];
    int nxt[200];
    int cur_len = len;
    int i, j, nxt_len;
    int steps = 0;

    for (i = 0; i < len; ++i) cur[i] = parts[i];

    while (cur_len > 1) {
        nxt_len = 0;
        for (i = 0; i < cur_len; ) {
            j = i + 1;
            while (j < cur_len && cur[j] == cur[i]) ++j;
            nxt[nxt_len++] = j - i;   /* run length of identical parts */
            i = j;
        }
        for (i = 0; i < nxt_len; ++i) cur[i] = nxt[i];
        cur_len = nxt_len;
        ++steps;
    }
    return steps;
}

/*  Recursive partition generator.
    remaining : sum still to be partitioned
    maxPart   : largest part allowed (enforces non‑increasing order)
    parts     : current partial partition
    len       : number of parts currently stored
    row       : array of counters for this row  (length 'n')
*/
static void recPartitions(int remaining, int maxPart,
                          int *parts, int len, seq_t *row, int n)
{
    if (remaining == 0) {
        int steps = resistance(parts, len);
        if (steps < n) row[steps]++;   /* store T(n, steps) */
        return;
    }
    for (int part = (remaining <= maxPart ? remaining : maxPart);
         part >= 1;
         --part)
    {
        parts[len] = part;
        recPartitions(remaining - part, part, parts, len + 1, row, n);
    }
}

/* Static cache of triangle rows.
   triangle[r][k] holds T(r, k), 0 <= k <= r-1. */
static seq_t **triangle = NULL;    /* array of pointers to rows */
static size_t  triangle_cap = 0;   /* capacity of the triangle array   */
static size_t  triangle_size = 1;  /* highest row currently stored (row 0 unused) */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Generate row 'n' and store it in the triangle cache. */
static void generate_row(size_t n)
{
    /* Allocate space for the row and initialise to zero. */
    seq_t *row = mem_new(seq_t, (size_t)n);
    if (!row) return;   /* silently fail – the calling function will return 0 */

    int parts[200];     /* temporary buffer for a partition                */
    recPartitions((int)n, (int)n, parts, 0, row, (int)n);

    /* Ensure that the triangle array has enough capacity. */
    if (triangle_cap == 0) {
        triangle_cap = 16;
        triangle = (seq_t **)malloc(triangle_cap * sizeof(seq_t *));
    }
    while (triangle_cap <= n) {
        triangle_cap *= 2;
        triangle = (seq_t **)realloc(triangle, triangle_cap * sizeof(seq_t *));
    }

    triangle[n] = row;
    if (n >= triangle_size) triangle_size = n + 1;
}

/* ------------------------------------------------------------------ */
/*  Ensure that rows up to 'target' are precomputed. */
static void compute_up_to(size_t target)
{
    if (triangle_size > target) return;
    for (size_t r = triangle_size; r <= target; ++r)
        generate_row(r);
}

/* ------------------------------------------------------------------ */
/*  Public entry point – returns the nth term of sequence A329746
   (1‑based index).  Behaviour for n < 1 is undefined; we return 0. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Find the row r such that r(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Position inside this row (0‑based). */
    idx_t prev_sum = (idx_t)(r - 1) * r / 2;
    idx_t k = n - prev_sum - 1;   /* column index */

    compute_up_to((size_t)r);

    return triangle[r][k];
}
