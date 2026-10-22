/*
 * A328407  G.f. A(x) satisfies: A(x) = A(x^2) + x * (1 + x) / (1 - x)^3.
 * https://oeis.org/A328407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(1) = 1; For n > 1: if n is even: a(n) = a(n/2) + 4 * (n/2)^2
 *            if n is odd: a(n) = n^2 (This follows from the recurrence
 *            a(2n)=a(n)+4n^2, a(2n+1)=(2n+1)^2.)
 * Range:     u8 n <= 15, u16 n <= 223, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, but guard against 0 if called */
        return 0;

    seq_t sum = 0;
    idx_t cur = n;

    /* accumulate contributions from each factor of 2 */
    while ((cur & 1ULL) == 0) {            /* cur is even */
        seq_t half = (seq_t)(cur >> 1);     /* half = cur/2   */
        sum += (seq_t)4 * half * half;     /* add 4*(half)^2 */
        cur >>= 1;                          /* move to cur/2   */
    }
    /* cur is now odd; add its square */
    seq_t odd = (seq_t)cur;
    return sum + odd * odd;
}
