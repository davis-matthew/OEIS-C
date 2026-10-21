/*
 * A271479  Number of steps for the trajectory of n under the map k ->
 *          A271478(k) to reach 1.
 * https://oeis.org/A271479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; a(2*n)=a(n)+1; a(2*n+1)=a(n+1)+3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _rec(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if ((n & (idx_t)1) == 0)
        return _rec(n >> 1) + (seq_t)1;
    else
        return _rec((n >> 1) + 1) + (seq_t)3;
}

seq_t a(idx_t n)
{
    return _rec(n);
}
