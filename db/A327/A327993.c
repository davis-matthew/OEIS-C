/*
 * A327993  a(n) = [x^n] ((x - 1)*(x + 1)*(2*x^2 - 1))/(2*x^4 + 4*x^3 - x^2 -
 *          3*x + 1).
 * https://oeis.org/A327993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) for n < 4 are 1, 3, 7, 20; for n = 4: a(4) =
 *            3*a(3)+a(2)-4*a(1)-2*a(0)+2; for n ≥ 5: a(n) =
 *            3*a(n-1)+a(n-2)-4*a(n-3)-2*a(n-4);
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial initial cases. */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)3;
    if (n == (idx_t)2) return (seq_t)7;
    if (n == (idx_t)3) return (seq_t)20;

    /* Variables holding the last four computed values: */
    seq_t v0 = (seq_t)1;   /* a(n-4) */
    seq_t v1 = (seq_t)3;   /* a(n-3) */
    seq_t v2 = (seq_t)7;   /* a(n-2) */
    seq_t v3 = (seq_t)20;  /* a(n-1) */
    seq_t vn;              /* current value a(n) */
    idx_t  i;

    /* Compute a(n) for n = 4 first (recurrence plus 2). */
    vn = 3 * v3 + v2 - 4 * v1 - 2 * v0 + (seq_t)2;
    /* If the requested n is 4, return immediately. */
    if (n == (idx_t)4) return vn;

    /* Shift the window for the next step. */
    v0 = v1;
    v1 = v2;
    v2 = v3;
    v3 = vn;

    /* From n = 5 onward use the homogeneous recurrence. */
    for (i = (idx_t)5; i <= n; ++i)
    {
        vn = 3 * v3 + v2 - 4 * v1 - 2 * v0;
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = vn;
    }

    return v3;
}
