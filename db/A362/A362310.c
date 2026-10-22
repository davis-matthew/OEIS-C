/*
 * A362310  Irregular triangle read by rows (row length A056220). Row n lists
 *          the integer solutions for x in the equation x - 10^n = x/y (x and
 *          y are integers).
 * https://oeis.org/A362310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the n-th term in the sequence obtained by * listing, for
 *            each r ≥ 0, the integer solutions x to * x – 10^r = x / y (x, y
 *            ∈ ℤ, y ≠ 0), * ordered ascending within each row. Each row r
 *            contains * 2·(r+1)^2 – 1 terms and the rows are concatenated to
 *            form * the overall sequence. * The algorith...
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow10, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow10.h"     /* seq_t pow10(idx_t exp) */
#include "../utils/seq_cmp.h"  /* int seq_cmp(const void*, const void*) */

seq_t a(idx_t n)
{
    /* ----- locate the row r containing index n ------------------- */
    uint64_t target = (uint64_t)n;          /* target index (0‑based) */
    uint64_t cum   = 0;                     /* cumulative count of terms so far */
    uint64_t r     = 0;                     /* row number */
    uint64_t row_len;

    for (;;)
    {
        row_len = 2ULL * (r + 1ULL) * (r + 1ULL) - 1ULL;
        if (cum + row_len > target)
            break;
        cum += row_len;
        ++r;
    }

    uint64_t pos_in_row = target - cum;     /* position within this row (0‑based) */

    /* ----- pre‑compute powers of 2 and 5 up to exponent r ---------- */
    size_t cnt = (size_t)(r + 1ULL);        /* number of exponents for 2 and 5 */
    seq_t *pow2 = (seq_t*)malloc(sizeof(seq_t) * cnt);
    seq_t *pow5 = (seq_t*)malloc(sizeof(seq_t) * cnt);
    if (!pow2 || !pow5)
    {
        free(pow2);
        free(pow5);
        return (seq_t)0;   /* allocation failure – unlikely in given limits */
    }

    pow2[0] = (seq_t)1;
    pow5[0] = (seq_t)1;
    for (size_t i = 1; i < cnt; ++i)
    {
        pow2[i] = pow2[i - 1] * (seq_t)2;
        pow5[i] = pow5[i - 1] * (seq_t)5;
    }

    /* ----- compute 10^r ------------------------------------------- */
    seq_t ten_pow = pow10(r);

    /* ----- generate all terms of the row -------------------------- */
    size_t len = (size_t)row_len;
    seq_t *row_vals = (seq_t*)malloc(sizeof(seq_t) * len);
    if (!row_vals)
    {
        free(pow2);
        free(pow5);
        return (seq_t)0;
    }

    size_t idx = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        for (size_t j = 0; j < cnt; ++j)
        {
            seq_t d   = pow2[i] * pow5[j];           /* positive divisor */
            seq_t inc = ten_pow / d;                 /* 10^r / d */

            /* value for positive divisor */
            row_vals[idx++] = ten_pow + inc;

            /* value for negative divisor (exclude d = -1) */
            if (!(i == 0 && j == 0))
                row_vals[idx++] = ten_pow - inc;
        }
    }

    /* ----- sort ascending and return the element ------------------ */
    qsort(row_vals, len, sizeof(seq_t), seq_cmp);
    seq_t result = row_vals[pos_in_row];

    /* ----- clean up ------------------------------------------------ */
    free(row_vals);
    free(pow2);
    free(pow5);

    return result;
}
