/*
 * A396637  Number of elements of geodesic length n in the braid group B_3.
 * https://oeis.org/A396637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - a(n-3) + 2*a(n-4) for n>=5 Initial
 *            values: a(0) = 1 a(1) = 4 a(2) = 12 a(3) = 30 a(4) = 68
 *            (computed from the rational generating function) The recurrence
 *            follows from the denominator of the rational g.f.: (2x^4 - x^3 -
 *            4x^2 + 4x - 1) * G(x) = 2x^4 + x^3 - 1
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)4;
    if (n == 2u) return (seq_t)12;
    if (n == 3u) return (seq_t)30;

    /* Compute a(4) from the formula with the constant 2 (p4 = 2) */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)4;   /* a(1) */
    seq_t a2 = (seq_t)12;  /* a(2) */
    seq_t a3 = (seq_t)30;  /* a(3) */
    seq_t a4 = 4*a3 - 4*a2 - a1 + 2*a0 - (seq_t)2; /* a(4) = 68 */

    if (n == 4u) return a4;

    /* Iterate from 5 up to n using the linear recurrence */
    idx_t i;
    for (i = 5; i <= n; ++i) {
        seq_t next = 4*a4 - 4*a3 - a2 + 2*a1;
        /* shift the window of previous values */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
