/*
 * A337154  a(n) = 4^n * (n!)^2 * Sum_{k=0..n} 1 / ((-4)^k * (k!)^2).
 * https://oeis.org/A337154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n^2*a(n-1) + (-1)^n, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A337154.
   Works for any seq_t that supports basic integer arithmetic
   (fixed width or arbitrary precision). */
seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;

    seq_t acc = (seq_t)1;          /* a(0) */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* acc = 4 * i * i * acc + (-1)^i */
        seq_t temp = acc * (seq_t)4;
        temp = temp * (seq_t)i;
        temp = temp * (seq_t)i;

        if (i & 1)                 /* i odd -> add (-1) */
            temp = temp - (seq_t)1;
        else
            temp = temp + (seq_t)1;

        acc = temp;
    }

    return acc;
}
