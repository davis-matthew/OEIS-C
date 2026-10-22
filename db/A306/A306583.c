/*
 * A306583  Positive integers that cannot be represented as a sum or
 *          difference of factorials of distinct integers.
 * https://oeis.org/A306583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   enumerate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) are the positive integers that cannot be expressed as a
 * sum or difference of factorials of distinct integers.
 *
 * This implementation precomputes all representable numbers up to 10! by
 * enumerating every choice of coefficients {-1,0,+1} for the factorials
 * 0! … 10!.  The remaining numbers are collected into an array; a(n) is
 * simply the (n‑1)st element of that array (OEIS uses 1‑based indexing).
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/enumerate.h"

static seq_t *nonrep_vals = NULL;   /* array of non‑representable values */
static size_t nonrep_cnt  = 0;       /* number of values stored */

/* Build the representation tables once */
static void build_tables(void)
{
    if (nonrep_cnt > 0)                /* already built */
        return;

    /* 10! = 3628800, enough to contain the first 10 000 terms */
    const idx_t NMAX = 3628800ULL;
    const int FACT_MAX = 10;           /* factorials 0! … 10! */

    /* factorials in seq_t (used only for compatibility) */
    seq_t fact_seq[FACT_MAX + 1];
    /* factorials in int64_t for recursion */
    int64_t fact_int[FACT_MAX + 1];

    fact_seq[0] = 1;
    fact_int[0] = 1;
    for (int i = 1; i <= FACT_MAX; ++i) {
        fact_seq[i] = fact_seq[i - 1] * (seq_t)i;
        fact_int[i] = fact_int[i - 1] * i;
    }

    /* boolean array of representable numbers */
    unsigned char *rep = (unsigned char*)calloc((size_t)NMAX + 1, 1);
    if (!rep)
        abort();                     /* allocation failure */

    enumerate(0, FACT_MAX, 0, fact_int, (size_t)NMAX, rep);

    /* Estimated number of non‑representables */
    size_t est = (size_t)NMAX - 200000;   /* rough lower bound  */
    nonrep_vals = (seq_t*)malloc(est * sizeof(seq_t));
    if (!nonrep_vals)
        abort();

    size_t pos = 0;
    for (size_t i = 1; i <= (size_t)NMAX; ++i) {
        if (!rep[i]) {
            if (pos == est) {               /* grow array if needed */
                est *= 2;
                nonrep_vals = (seq_t*)realloc(nonrep_vals, est * sizeof(seq_t));
                if (!nonrep_vals)
                    abort();
            }
            nonrep_vals[pos++] = (seq_t)i;
        }
    }
    nonrep_cnt = pos;
    free(rep);
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for n < 1 */
        return 0;
    build_tables();
    if ((size_t)(n - 1) < nonrep_cnt)
        return nonrep_vals[n - 1];
    /* Request beyond pre‑computed range – this should not happen for
       the intended use (indices up to ~10 000).  Return 0 as a safe default. */
    return 0;
}
