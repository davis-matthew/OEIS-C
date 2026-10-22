/*
 * A383958  Sum of the legs of the unique primitive Pythagorean triple (a,b,c)
 *          such that (a-b+c)/2 = A000108(n) and its long leg and hypotenuse
 *          are consecutive natural numbers.
 * https://oeis.org/A383958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A000108(n)^2 - 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a(n) by computing the n-th Catalan number C_n = A000108(n)
   using the recurrence C_{i+1} = C_i * 2*(2*i+1) / (i+2), then returning
   2*C_n*C_n - 1.  No external state or allocation is needed. */

seq_t a(idx_t n)
{
    seq_t c = (seq_t)1;                         /* C_0 = 1 */
    for (idx_t i = (idx_t)1; i <= n; ++i) {     /* compute C_i for i=1..n */
        seq_t mult = (seq_t)(2 * (2 * (i - (idx_t)1) + (idx_t)1));
        c = (c * mult) / (i + (idx_t)1);      /* exact division */
    }
    seq_t res = (seq_t)2 * c * c - (seq_t)1;    /* a(n) = 2*C_n^2 - 1 */
    return res;
}
