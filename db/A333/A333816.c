/*
 * A333816  Number of ways to write n as the difference of two hexagonal
 *          numbers.
 * https://oeis.org/A333816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as H_j - H_i where H_k =
 *            k(2k-1) are hexagonal numbers with indices i,j >= 0. The number
 *            of solutions equals the number of pairs of integers (d, m) with
 *            n = d * m, m odd, such that d and (m+1)/2 have the same parity
 *            and (m+1)/2 >= d. This is equivalent to enumerat...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t count = 0;
    /* iterate over odd divisors m of n */
    for (idx_t m = 1; m <= n; m += 2) {
        if (n % m == 0) {
            idx_t d = n / m;            /* d = a - b (>= 0)   */
            idx_t S = (m + 1) / 2;      /* S = a + b          */
            /* parity check: d and S must have same parity */
            if ( (d & 1) == (S & 1) ) {
                /* ensure b = (S - d)/2 >= 0  ->  S >= d */
                if (S >= d)
                    ++count;
            }
        }
    }
    return count;
}
