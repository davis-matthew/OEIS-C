/*
 * A330767  a(n) = 25*a(n-1) + a(n-2), starting with a(0) = 2 and a(1) = 25.
 * https://oeis.org/A330767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 25*a(n-1) + a(n-2), a(0)=2, a(1)=25 */

/* Simple 2x2 matrix structure for the linear recurrence */
struct mat
{
    seq_t a00, a01, a10, a11;
};

/* Matrix multiplication (returns the product of two matrices) */
static struct mat mat_mul(const struct mat *x, const struct mat *y)
{
    struct mat z;
    z.a00 = x->a00 * y->a00 + x->a01 * y->a10;
    z.a01 = x->a00 * y->a01 + x->a01 * y->a11;
    z.a10 = x->a10 * y->a00 + x->a11 * y->a10;
    z.a11 = x->a10 * y->a01 + x->a11 * y->a11;
    return z;
}

/* Computes the n-th term of A330767 */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)25;

    /* Identity matrix */
    struct mat result = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 };
    /* Transformation matrix for a(n) = 25*a(n-1)+a(n-2) */
    struct mat base = { (seq_t)25, (seq_t)1, (seq_t)1, (seq_t)0 };

    idx_t e = n - 1;
    while (e)
    {
        if (e & 1) result = mat_mul(&result, &base);
        base = mat_mul(&base, &base);
        e >>= 1;
    }

    /* Multiply the powered matrix by the initial vector [a(1); a(0)] = [25; 2] */
    seq_t ans = result.a00 * (seq_t)25 + result.a01 * (seq_t)2;
    return ans;
}
