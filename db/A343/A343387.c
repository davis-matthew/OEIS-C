/*
 * A343387  Number of ways to write n as x^2 + [y^2/2] + [z^4/8], where [.] is
 *          the floor function, x is a nonnegative integer, and y and z are
 *          positive integers.
 * https://oeis.org/A343387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 7273, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force counting of representations of n as
   n = x^2 + floor(y^2/2) + floor(z^4/8), x>=0, y>0, z>0 */

seq_t a(idx_t n)
{
    seq_t count = 0;                     /* result */

    for (idx_t x = 0; (seq_t)x*x <= n; ++x)
    {
        seq_t nx = (seq_t)n;             /* cast once */
        seq_t x2 = (seq_t)x*x;
        seq_t rem_x = nx - x2;           /* n - x^2 */

        /* y loop: floor(y^2/2) <= rem_x  <=>  y^2 < 2*(rem_x+1)   */
        for (idx_t y = 1; (seq_t)y*y < 2*(rem_x + 1); ++y)
        {
            seq_t y2 = (seq_t)y*y;
            seq_t valY = y2 / 2;         /* floor(y^2/2) */
            seq_t rem_y = rem_x - valY;  /* remaining after y */

            /* z loop: floor(z^4/8) <= rem_y  <=>  z^4 < 8*(rem_y+1) */
            for (idx_t z = 1; ; ++z)
            {
                seq_t z2 = (seq_t)z*z;
                seq_t z4 = z2*z2;               /* z^4   */
                seq_t valZ = z4 / 8;            /* floor(z^4/8) */
                if (valZ > rem_y) break;        /* no more candidates */
                if (valZ == rem_y) count++;     /* found a representation */
            }
        }
    }
    return count;
}
