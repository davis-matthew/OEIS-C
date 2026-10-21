/*
 * A295287  Sum of the products of the smaller and larger parts of the
 *          partitions of n into two parts with the smaller part even.
 * https://oeis.org/A295287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} i*(n-i)*((i+1) mod 2)
 * Range:     u8 n <= 18, u16 n <= 115, u32 n <= 4687, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e. sum over even i ≤ floor(n/2) of i*(n-i)) */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t sn  = (seq_t)n;        /* cast index to seq_t for safe arithmetic */

    /* iterate over even parts i = 2,4,6,... <= n/2 */
    for (idx_t i = 2; i <= n / 2; i += 2) {
        seq_t si = (seq_t)i;
        sum += si * (sn - si);
    }
    return sum;
}
