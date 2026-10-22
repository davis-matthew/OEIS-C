/*
 * A394274  Number of induced cubes in the n-Sierpinski carpet graph.
 * https://oeis.org/A394274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (40 - 112*3^(n-1) + 107*8^(n-1))/35 Recurrence: a(1)=1,
 *            a(2)=16, a(3)=168, a(n)=12*a(n-1) - 35*a(n-2) + 24*a(n-3) for
 *            n≥4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            return (seq_t)0;   /* just in case */
    if (n == 1)            return (seq_t)1;
    if (n == 2)            return (seq_t)16;
    if (n == 3)            return (seq_t)168;

    /* a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)16;  /* a(2) */
    seq_t a3 = (seq_t)168; /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        /* compute 12*a(n-1) - 35*a(n-2) + 24*a(n-3) */
        seq_t t1 = a3 * (seq_t)12;  /* 12*a(n-1) */
        seq_t t2 = a2 * (seq_t)35;  /* 35*a(n-2) */
        seq_t t3 = a1 * (seq_t)24;  /* 24*a(n-3) */

        seq_t tmp   = t1 + t3;      /* 12*a(n-1) + 24*a(n-3) */
        seq_t curr  = tmp - t2;     /* subtract 35*a(n-2); guaranteed non‑negative */

        /* shift for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = curr;
    }
    return a3;  /* a(n) */
}
