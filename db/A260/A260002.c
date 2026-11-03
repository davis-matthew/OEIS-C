/*
 * A260002  Sudan Numbers: a(n)= f(n,n,n) where f is the Sudan function.
 * https://oeis.org/A260002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, bref
 * Formula:   a(n) = f(n,n,n) where the Sudan function f is defined by
 *            f(0,x,y) = x + y f(z,x,0) = x f(z,x,y) = f(z-1, f(z,x,y-1),
 *            f(z,x,y-1) + y) (z>0, y>0)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Validated: partial, pass, 3 terms through n = 2 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t f_rec(idx_t z, seq_t x, seq_t y)
{
    /* base case: third argument zero -> return second argument */
    if (y == (seq_t)0)
        return x;

    /* base case: first argument zero -> return sum of the last two */
    if (z == (idx_t)0)
        return x + y;

    /* recursive step: compute f(z,x,y-1) once, then use it twice */
    seq_t m = f_rec(z, x, y - (seq_t)1);   /* f(z,x,y-1)   */
    seq_t yplus = m + y;                   /* f(z,x,y-1)+y */
    return f_rec(z - (idx_t)1, m, yplus);
}

seq_t a(idx_t n)
{
    return f_rec((idx_t)n, (seq_t)n, (seq_t)n);
}
