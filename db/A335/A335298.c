/*
 * A335298  a(n) is the squared distance between the points P(n) and P(0) on a
 *          plane, n >= 0, such that the distance between P(n) and P(n+1) is
 *          n+1 and, going from P(n) to P(n+2), a 90-degree left turn is taken
 *          in P(n+1).
 * https://oeis.org/A335298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A335298
   Formula: 
      Let k = floor(n/4).
      a(4*k)   = 8 * k^2
      a(4*k+1) = 8 * k^2 + 4*k + 1
      a(4*k+2) = 8 * k^2 + 12*k + 5
      a(4*k+3) = 8 * (k+1)^2
   This directly follows the OEIS entry and is computed in O(1) time. */
seq_t a(idx_t n)
{
    /* Compute the quotient and remainder of n divided by 4 */
    seq_t    k = (seq_t)(n >> 2);      /* k = floor(n/4) */
    idx_t    r = n & 3ULL;             /* r = n % 4 */

    switch (r)
    {
        case 0:
            /* a(4*k) = 8 * k^2 */
            return (seq_t)8 * k * k;

        case 1:
            /* a(4*k+1) = 8 * k^2 + 4*k + 1 */
            return (seq_t)8 * k * k + (seq_t)4 * k + (seq_t)1;

        case 2:
            /* a(4*k+2) = 8 * k^2 + 12*k + 5 */
            return (seq_t)8 * k * k + (seq_t)12 * k + (seq_t)5;

        case 3:
        {
            /* a(4*k+3) = 8 * (k+1)^2 */
            seq_t kp1 = k + (seq_t)1;
            return (seq_t)8 * kp1 * kp1;
        }
    }

    /* This point is unreachable; return 0 to silence compiler warnings. */
    return (seq_t)0;
}
