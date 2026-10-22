/*
 * A342134  Square array T(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of g.f. 1/(1 - 2*k*x - k*x^2).
 * https://oeis.org/A342134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(0,k)=1, T(1,k)=2k, T(n,k)=k*(2*T(n-1,k)+T(n-2,k)) for n>1. The
 *            sequence a(n) enumerates T(row,col) by successive anti‑diagonals
 *            with row increasing first: for d=0,1,2,... list (i,j) with
 *            i=0..d, j=d-i
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 128, u64 n <= 315,
 *            u128 n <= 847, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, etc., if needed */

static seq_t T_value(idx_t row, idx_t col)
{
    /* Minor guard: for k=0 all later values are 0, but recurrence works anyway. */
    if (row == 0)
        return (seq_t)1;
    if (row == 1)
        return (seq_t)2 * (seq_t)col;

    seq_t t0 = (seq_t)1;                /* T(0,col) */
    seq_t t1 = (seq_t)2 * (seq_t)col;   /* T(1,col) */

    for (idx_t r = 2; r <= row; ++r) {
        seq_t prod = (seq_t)col * (((seq_t)2 * t1) + t0);
        t0 = t1;
        t1 = prod;
    }
    return t1;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Find diagonal d such that total elements up to previous diagonal <= n <
       total up to current diagonal.
       total(d) = (d+1)(d+2)/2 ; solve for d via loop (d grows only ~sqrt(n)). */
    idx_t d = 0;
    idx_t count = 1;          /* total elements up to current diagonal (d=0) */
    while (count <= n) {      /* move to next diagonal until n is inside */
        ++d;
        count += d + 1;       /* add (d+1) elements of new diagonal */
    }

    /* offset inside diagonal */
    idx_t prev_count = count - (d + 1);
    idx_t offset = n - prev_count;   /* 0 <= offset <= d */

    idx_t row = offset;          /* i */
    idx_t col = d - offset;      /* j */

    return T_value(row, col);
}
