/*
 * A276382  a(1) = 1, and a(n) = a(n-1) + floor(3*n/2) + 1 for n >= 2.
 * https://oeis.org/A276382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 169, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A276382: a(1) = 1,
   a(n) = a(n-1) + floor(3*n/2) + 1  (n ≥ 2)
   Equivalent closed form (valid for n ≥ 1):
   a(n) = floor((3*(n+1)^2 – 7) / 4)
*/
seq_t a(idx_t n)
{
    /* If the caller passes n == 0, the OEIS offset is undefined.
       Return 0 to avoid unsigned underflow. */
    if (n == 0) return 0;

    seq_t N = (seq_t)n;
    seq_t term = N + 1;                 /* (n+1) */
    seq_t temp = 3 * term * term;       /* 3*(n+1)^2 */
    temp -= 7;                          /* 3*(n+1)^2 – 7 */
    return temp / 4;                    /* floor division by 4 */
}
