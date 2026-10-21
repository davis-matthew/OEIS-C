/*
 * A293325  Least integer k such that k/2^n > sqrt(3).
 * https://oeis.org/A293325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( sqrt(3) * 2^n )
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let s(n) = floor( sqrt(3) * 2^n ),
   r(n) = 3·4^n – s(n)^2  (0 ≤ r(n) < 2·s(n)+1).
   Then
      s(n+1) = 2·s(n) + ( r(n) >= s(n)+1 ? 1 : 0 )
      r(n+1) = 4·r(n) - ( r(n) >= s(n)+1 ? (4·s(n)+1) : 0 )
   and a(n) = s(n) + 1. */

seq_t a(idx_t n)
{
    /* initial values for n = 0 */
    seq_t s = 1;               /* s(0) = floor(sqrt(3)) = 1          */
    seq_t r = 2;               /* r(0) = 3·4^0 - s(0)^2 = 2          */

    for (idx_t i = 0; i < n; ++i) {
        /* decide whether to add 1 to the doubled s   */
        idx_t delta = (r >= s + 1) ? 1 : 0;

        /* compute next s and r                      */
        seq_t next_s = (s << 1) + delta;           /* 2*s + delta */
        seq_t next_r = r << 2;                    /* 4*r          */
        if (delta) {
            next_r -= ((s << 2) + 1);              /* subtract 4*s + 1 */
        }

        s = next_s;
        r = next_r;
    }

    return s + 1;          /* a(n) = floor(sqrt(3)*2^n ) + 1 */
}
