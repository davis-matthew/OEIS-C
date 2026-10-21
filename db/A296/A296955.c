/*
 * A296955  Sum of the smaller parts of the partitions of n into two distinct
 *          parts such that the smaller part divides the larger.
 * https://oeis.org/A296955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of the divisors d of n with 2*d < n
 * Range:     u8 n <= 179, u16 n <= 9843, u32 n <= 9843, u64 n <= 9843,
 *            u128 n <= 9843, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently, sum of all divisors of n that are strictly less than n/2) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over divisor pairs i and n/i without overflow */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0) continue;   /* i is not a divisor */
        /* first divisor of the pair */
        if ((i << 1) < n)          /* 2*i < n  (fits in idx_t) */
            sum += (seq_t)i;
        /* second divisor of the pair, distinct from the first */
        idx_t j = n / i;
        if (j != i && (j << 1) < n)
            sum += (seq_t)j;
    }
    return sum;
}
