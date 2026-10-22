/*
 * A368684  Number of partitions of n into 2 parts such that the smaller part
 *          divides both n and floor(n/2).
 * https://oeis.org/A368684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = d(floor((n+1)/2))^((n+1) mod 2), with a(1)=0. d(k) =
 *            number of divisors of k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the divisor count of a small integer n (n <= 10^4 is enough
   for the performance requirements).  The result fits into seq_t. */
static seq_t divisor_count_u64(idx_t n)
{
    seq_t cnt = 1;
    idx_t tmp = n;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            seq_t exp = 0;
            while (tmp % p == 0) {
                ++exp;
                tmp /= p;
            }
            cnt *= (exp + 1);
        }
    }
    if (tmp > 1) /* the remaining prime factor */
        cnt *= 2;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 1u)
        return 0; /* offset 1 : a(1) = 0 */

    /* m = floor((n+1)/2) */
    idx_t m = (n + 1u) / 2u;

    /* exponent = (n+1) mod 2 : 1 for even n, 0 for odd n */
    if ((n + 1u) & 1u)
        return divisor_count_u64(m);
    else
        return 1;
}
