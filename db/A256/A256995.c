/*
 * A256995  Square array A(row,col) read by antidiagonals: A(row,1) =
 *          A055938(row), and for col > 1, A(row,col) = A005187(A(row,col-1)).
 * https://oeis.org/A256995
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the element on row = offset‐1 and col = diagonal‑offset
 *            of the square array A with A(row,1) = A055938(row) (numbers not
 *            in the range of f(m)=2*m‑popcount(m)) A(row,col) =
 *            A005187(A(row,col‑1)) = 2*x‑popcount(x) The array is read by
 *            antidiagonals starting with A(1,1) as a(2)
 * Range:     u8 n <= 38, u16 n <= 138, u32 n <= 530, u64 n <= 2082,
 *            u128 n <= 8258, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* popcount for any unsigned seq_t (works for 8/16/32/64/128 and arbitrary‑precision) */
static unsigned popcount_generic(seq_t x)
{
    unsigned c = 0;
    while (x != (seq_t)0) {
        x &= x - (seq_t)1;
        ++c;
    }
    return c;
}

/* f(x) = A005187(x) = 2*x - popcount(x) */
static seq_t f_func(seq_t x)
{
    return ((seq_t)2 * x) - (seq_t)popcount_generic(x);
}

/* ----------  seed array (A055938)  ---------- */

static seq_t *seed_cache = NULL;       /* dynamic array of seeds (1‑based) */
static size_t seed_cache_cnt = 0;      /* number of seeds stored    */
static size_t seed_cache_cap = 0;      /* capacity of seed_cache   */

static idx_t next_m = 1;              /* next m to generate f value   */
static seq_t next_f = (seq_t)0;       /* f(next_m)                    */
static idx_t next_candidate = 2;      /* candidate number to test     */

/* ensure that seed_cache contains at least need rows */
static void ensure_seed_cache(size_t need_rows)
{
    if (!seed_cache) {                       /* first time initialisation   */
        seed_cache_cap = 16;
        seed_cache = (seq_t *)malloc(seed_cache_cap * sizeof(seq_t));
        if (!seed_cache) return;             /* graceful failure (should not happen) */
        seed_cache_cnt = 0;
        next_m = 1;
        next_f = f_func((seq_t)1);           /* f(1) = 1 */
        next_candidate = 2;
    }

    while (seed_cache_cnt < need_rows) {
        /* advance f values until next_f >= next_candidate */
        while (next_f < next_candidate) {
            ++next_m;
            next_f = f_func((seq_t)next_m);
        }

        if (next_f == next_candidate) {      /* candidate is an image → skip */
            ++next_candidate;
            continue;
        }

        /* candidate is a seed → store it */
        if (seed_cache_cnt == seed_cache_cap) {
            size_t new_cap = seed_cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(seed_cache, new_cap * sizeof(seq_t));
            if (!tmp) return;               /* graceful failure              */
            seed_cache = tmp;
            seed_cache_cap = new_cap;
        }
        seed_cache[seed_cache_cnt++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* ----------  main sequence function  ---------- */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset is 2 – behaviour for n<2 is unspecified */

    /* rank among the antidiagonal sequence after the offset (n-2) */
    idx_t rank = n - 2;

    /* find diagonal d such that cumulative elements up to d-1 <= rank < cumulative up to d */
    idx_t d = 2;                   /* minimal diagonal (row+col = 2)          */
    idx_t cum = 1;                 /* total elements up to current d-1        */
    while (cum <= rank) {
        ++d;
        cum += (d - 1);            /* cum = (d-1)*d/2                         */
    }

    idx_t prev_cum = cum - (d - 1);            /* elements before current diagonal    */
    idx_t offset = rank - prev_cum;            /* 0‑based offset within diagonal     */
    idx_t row = offset + 1;                    /* 1‑based row index                  */
    idx_t col = d - offset - 1;                /* 1‑based column index               */

    /* obtain the first column value for this row */
    ensure_seed_cache((size_t)row);
    seq_t value = seed_cache[row - 1];

    /* apply A005187 (f) col-1 times to reach column */
    for (idx_t step = 1; step < col; ++step) {
        value = f_func(value);
    }

    return value;
}
