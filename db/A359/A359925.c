/*
 * A359925  Numbers with easy multiplication table - the first 9 multiples of
 *          these numbers can be derived by either incrementing or
 *          decrementing the corresponding digits from the previous multiple.
 * https://oeis.org/A359925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10*a(floor(n/2)) + 2*(n mod 2) - 1 with a(0)=0
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=1, a(2)=9, a(3)=11, a(4)=89, … */

static seq_t a_rec(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t half = a_rec(n >> 1);
    if (n & 1U)
        return half * 10 + 1;
    else
        return half * 10 - 1;
}

seq_t a(idx_t n)
{
    return a_rec(n);
}
