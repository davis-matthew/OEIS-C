/*
 * A332547  a(n) = largest odd divisor d < n of n*(n+1)/2.
 * https://oeis.org/A332547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest odd divisor d < n of n*(n+1)/2
 * Range:     u8 n <= 22, u16 n <= 691, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) {          /* offset starts at 1, but guard against bad input */
        return 0;
    }

    seq_t n_seq = (seq_t)n;
    seq_t T = n_seq * (n_seq + 1) / 2;   /* triangular number */

    /* strip powers of 2 to obtain the odd part */
    seq_t odd_part = T;
    while ((odd_part & 1) == 0) {
        odd_part >>= 1;
    }

    /* search for the largest odd divisor less than n, scanning odd
       numbers downward from n-1 */
    seq_t d = n_seq - 1;
    if ((d & 1) == 0) {          /* make it odd, if necessary */
        if (d == 0) return 1;    /* case n==1 */
        d -= 1;
    }

    for (; d >= 1; d -= 2) {
        if (odd_part % d == 0) {
            return d;
        }
    }

    /* 1 always divides, thus this is the fallback */
    return 1;
}
