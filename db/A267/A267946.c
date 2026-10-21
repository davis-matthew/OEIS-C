/*
 * A267946  Number of n X 1 0..2 arrays with every repeated value in every row
 *          and column one larger mod 3 than the previous repeated value, and
 *          upper left element zero.
 * https://oeis.org/A267946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 6*a(n-3) with initial values a(1)=1,
 *            a(2)=3, a(3)=8
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of A267946.  The function works for any
   idx_t and seq_t types that support the required arithmetic
   operators.  No dynamic allocation is used, so it compiles both
   under C11 and C++17. */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1. */
    if (n < 1) return 0;

    /* Handle the first three terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)8;

    /* Recurrence requires only the last three values. */
    seq_t t1 = (seq_t)1;   /* a(n-3) */
    seq_t t2 = (seq_t)3;   /* a(n-2) */
    seq_t t3 = (seq_t)8;   /* a(n-1) */
    seq_t tn = 0;          /* a(n) */

    for (idx_t k = 4; k <= n; ++k)
    {
        tn = 3 * t3 + t2 - 6 * t1;
        t1 = t2;   /* shift window */
        t2 = t3;
        t3 = tn;
    }
    return tn;
}
