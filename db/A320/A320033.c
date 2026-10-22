/*
 * A320033  a(n) is the number of integer partitions of n with largest part <=
 *          5 for which the index of the seaweed algebra formed by the integer
 *          partition paired with its weight is 0.
 * https://oeis.org/A320033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = { 7 if n%4==1 3 if n%4==2 5 if n%4==3 3 if n%4==0 } for
 *            n>20 a(n) = a(n-4) for n>24. For 1≤n≤20 the values are taken
 *            from a fixed table
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 20 terms of the sequence (offset 1) */
    static const seq_t table[20] = {
        1, 1, 1, 2, 3, 3, 4, 5, 6, 6,
        4, 6, 7, 5, 5, 6, 7, 4, 5, 5
    };

    if (n <= 20)          /* use the table for n=1…20 */
        return table[n - 1];

    /* for n > 20, use the periodic pattern of period 4 */
    switch (n % 4) {
        case 1: return 7;   /* n ≡ 1 (mod 4) */
        case 2: return 3;   /* n ≡ 2 (mod 4) */
        case 3: return 5;   /* n ≡ 3 (mod 4) */
        default: return 3;  /* n ≡ 0 (mod 4) */
    }
}
