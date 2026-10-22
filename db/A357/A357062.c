/*
 * A357062  Number of ordered solutions to n = x*y*z + x + y + z in positive
 *          integers.
 * https://oeis.org/A357062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered positive integer solutions to n = x*y*z
 *            + x + y + z in positive integers
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* iterate over all possible products p = x*y */
    for (idx_t p = 1; p <= n; ++p) {
        idx_t half = (idx_t)0;
        /* find all divisors d of p */
        for (idx_t d = 1; d * d <= p; ++d) {
            if (p % d != 0) continue;

            idx_t q = p / d;          /* q = p/d */
            idx_t s = d + q;          /* x + y for this divisor pair */
            idx_t denom = p + 1;      /* (p+1) in the formula for z */
            idx_t numer = n > s ? n - s : 0;

            /* check the divisor d */
            if (numer >= denom && numer % denom == 0) {
                idx_t z = numer / denom;
                if (z >= 1) ++count;
            }

            /* if d and q are different, d and q give
               distinct ordered pairs (x,y) and (y,x) */
            if (d != q) {
                /* s is the same, proces the other divisor q */
                if (numer >= denom && numer % denom == 0) {
                    idx_t z = numer / denom;
                    if (z >= 1) ++count;
                }
            }
        }
    }
    return count;
}
