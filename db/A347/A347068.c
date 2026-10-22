/*
 * A347068  Rectangular array (T(n,k)), by downward antidiagonals: T(n,k) =
 *          position of k in the ordering of {h*r^m, r = 1/(golden ratio), h
 *          >= 1, 0 <= m <= n}.
 * https://oeis.org/A347068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th term of the array T(n,k) read along downward
 *            antidiagonals, where T(n,k) = position of the integer k in the
 *            ordered set { h * r^m | h ≥ 1 , 0 ≤ m ≤ n }, r = 1/φ (φ =
 *            (1+√5)/2)
 * Range:     u8 n <= 6, u16 n <= 52, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Helper:  floor( N * φ ) for arbitrary‑precision integer N. */
static seq_t floor_N_phi(seq_t N)
{
    if (N == 0)
        return 0;
    seq_t inside = N * N * 5;        /* inside = 5·N² */
    seq_t sq     = floor_sqrt(inside);   /* floor( √(5·N²) ) = floor(N·√5) */
    return (N + sq) / 2;               /* floor( (N + floor(N√5)) / 2 ) */
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal (row‑col) that contains index n. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 < n)
        ++d;
    idx_t prev_total = d * (d + 1) / 2;
    idx_t offset = n - prev_total - 1;   /* 0‑based inside the diagonal   */
    idx_t row = 1 + offset;              /* n in the definition of T      */
    idx_t col = 1 + d - offset;          /* k in the definition           */

    seq_t total = 0;
    seq_t col_seq = (seq_t)col;

    /* a_m, b_m store the integers in φ^m = a_m + b_m·φ  */
    seq_t a_m = 1;   /* φ⁰ = 1                                 */
    seq_t b_m = 0;   /* φ⁰ = 1 + 0·φ                           */

    for (idx_t m = 0; m <= row; ++m)
    {
        seq_t term = col_seq * a_m;           /* col * a_m                */

        if (b_m != 0)                         /* add floor(col·b_m·φ)     */
        {
            seq_t N = col_seq * b_m;
            term += floor_N_phi(N);
        }

        total += term;

        /* recurrence a_{m+1} = b_m ,  b_{m+1} = a_m + b_m   */
        seq_t next_a = b_m;
        seq_t next_b = a_m + b_m;
        a_m = next_a;
        b_m = next_b;
    }

    return total;
}
