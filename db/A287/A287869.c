/*
 * A287869  Wythoff array with one extra column, read by antidiagonals
 *          downwards.
 * https://oeis.org/A287869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A287869 – Wythoff array with one extra column, read by antidiagonals downwards.
    The sequence is obtained by scanning the extended Wythoff array,
    starting from the first “real” column (column 1, the lower Wythoff
    sequence).  For each non‑negative integer i (row number) the
    columns are:
          column 0 – row index  (ignored in the scan)
          column 1 – lower Wythoff number L_i = ⌊(i+1)φ⌋
          column 2 – a₁ = L_i + i
          column 3 – a₂ = a₁ + a₀
          …
    The value in column j (j ≥ 1) is a_{j‑1} where a₀ = L_i, a₁ = L_i+i and
    a_{k+1} = a_k + a_{k‑1}.  The terms of A287869 are the entries of this
    triangular array read by antidiagonals (s = i + j) in order of increasing s
    and, for each s, of increasing i (i.e. decreasing j).  The offset is 1
    (a(1) is the first term in the sequence).  This function computes a(n)
    for any n ≥ 1 in O(√n) time without any external tables.  It works
    for all the supported integer types (uint8_t … unsigned __int128 or
    arbitrary‑precision classes). */

#include <stdint.h>

/* Golden ratio constant, accurate to 20 decimal places.  No sqrt is needed. */
static const double PHI = 1.6180339887498948482045868;

/* Compute the lower Wythoff number L_i = ⌊(i+1)φ⌋   (i ≥ 0). */
static inline seq_t lower_wythoff(idx_t i)
{
    double d = (double)(i + 1) * PHI;         /* d is ≤ 2·10^7 for i < 10^7  */
    return (seq_t)(uint64_t)d;                /* truncate toward zero   */
}

/* Return the n‑th term of A287869 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* out of range, safety */

    /* Find the antidiagonal s such that T(s) = s·(s+1)/2 ≥ n. */
    idx_t s = 0;
    idx_t tri = 0;                           /* triangular number T(s)   */
    while (tri < n)                          /* s about √(2n), so loop is fast */
    {
        ++s;
        tri = (idx_t)(s * (s + 1) / 2);
    }

    idx_t prevTri = (idx_t)((s - 1) * s / 2);
    idx_t k = n - prevTri - 1;                /* 0‑based position inside diagonal */
    idx_t i = k;                              /* row index   */
    idx_t j = s - k;                          /* column index (≥1) */

    seq_t a0 = lower_wythoff(i);              /* column 1   */
    if (j == 1) return a0;                   /* value L_i  */

    seq_t a1 = a0 + (seq_t)i;                 /* column 2   */
    if (j == 2) return a1;                   /* value a₁   */

    /* Fibonacci recurrence to reach column j. */
    seq_t prev = a0;
    seq_t cur  = a1;
    for (idx_t t = 2; t < j; ++t) {
        seq_t next = prev + cur;
        prev = cur;
        cur  = next;
    }
    return cur;
}
