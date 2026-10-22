/*
 * A319981  a(n) is the number of integer partitions of n with largest part <=
 *          3 for which the index of the seaweed algebra formed by the integer
 *          partition paired with its weight is 0.
 * https://oeis.org/A319981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n > 12, a(n) = 2 if n is odd, a(n) = 0 if n is even. For n ≤
 *            13, use the following explicit values: {1, 1, 1, 2, 2, 1, 2, 1,
 *            2, 1, 2, 1, 2}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Handle the small cases explicitly. */
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, return 0 by convention */

    if (n <= 13) {
        static const seq_t tbl[13] = {
            (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)1,
            (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)1,
            (seq_t)2
        };
        return tbl[n - 1];
    }

    /* For n > 13, the sequence alternates 2 (odd) and 0 (even). */
    return (n & 1) ? (seq_t)2 : (seq_t)0;
}
