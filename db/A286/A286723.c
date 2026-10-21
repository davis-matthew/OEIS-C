/*
 * A286723  Column k = 1 of the triangle A225471; Sheffer ((1 - 3*x)^(-3/4),
 *          (-1/4)*log(1 - 4*x)).
 * https://oeis.org/A286723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; For n ≥ 1: a(n) = (3 + 4*n) * a(n-1) + ∏_{i=0}^{n-1}
 *            (3 + 4*i)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence follows from the elementary symmetric sums of the
   arithmetic progression 3,7,11,…,1+4*n. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t a_val = (seq_t)1;               /* a(0) */
    seq_t P      = (seq_t)3;              /* ∏_{i=0}^{0} (3+4*i) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t xi   = (seq_t)3 + (seq_t)4 * (seq_t)i; /* 3 + 4*i */
        seq_t next = xi * a_val + P;                /* a(i) = xi*a(i-1)+P */
        P  *= xi;                                     /* update product */
        a_val = next;
    }

    return a_val;
}
