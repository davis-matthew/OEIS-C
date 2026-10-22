/*
 * A376838  Phase shift (original name "sfasamento") of the tetration base
 *          10*n at height 2.
 * https://oeis.org/A376838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least significant non‑zero digit of n^(n*10)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If the caller passes an index below the offset, return 0 safely. */
    if (n == 0)
        return (seq_t)0;

    /* Base: n with trailing zeros removed. */
    seq_t m = (seq_t)n;
    while (m % (seq_t)10 == (seq_t)0)
        m /= (seq_t)10;

    /* Last non‑zero digit of the reduced base. */
    unsigned int d = (unsigned int)(m % (seq_t)10);

    /* Immediate cases that don't depend on the exponent. */
    if (d == 1 || d == 9)
        return (seq_t)1;
    if (d == 5)
        return (seq_t)5;
    if (d == 6)
        return (seq_t)6;

    /* Exponent e = n*10.  Only e mod 4 is needed for the cyclic patterns. */
    unsigned int n_mod4 = (unsigned int)(n % 4);
    unsigned int exp_mod4 = (n_mod4 * 2) % 4;   /* 10 mod 4 == 2 */
    if (exp_mod4 == 0)
        exp_mod4 = 4;                           /* map 0 to the full cycle length */

    /* Return the digit according to the last‑digit cycle. */
    switch (d) {
    case 2:
        /* Cycle: 2,4,8,6 for exponents 1,2,3,4. */
        if (exp_mod4 == 1) return (seq_t)2;
        if (exp_mod4 == 2) return (seq_t)4;
        if (exp_mod4 == 3) return (seq_t)8;
        return (seq_t)6;                       /* exp_mod4 == 4 */
    case 3:
        /* Cycle: 3,9,7,1. */
        if (exp_mod4 == 1) return (seq_t)3;
        if (exp_mod4 == 2) return (seq_t)9;
        if (exp_mod4 == 3) return (seq_t)7;
        return (seq_t)1;                       /* exp_mod4 == 4 */
    case 4:
        /* 4^k alternates 4,6.  Odd -> 4, Even -> 6. */
        return (exp_mod4 % 2 == 1) ? (seq_t)4 : (seq_t)6;
    case 7:
        /* Cycle: 7,9,3,1. */
        if (exp_mod4 == 1) return (seq_t)7;
        if (exp_mod4 == 2) return (seq_t)9;
        if (exp_mod4 == 3) return (seq_t)3;
        return (seq_t)1;                       /* exp_mod4 == 4 */
    case 8:
        /* Cycle: 8,4,2,6. */
        if (exp_mod4 == 1) return (seq_t)8;
        if (exp_mod4 == 2) return (seq_t)4;
        if (exp_mod4 == 3) return (seq_t)2;
        return (seq_t)6;                       /* exp_mod4 == 4 */
    default:
        return (seq_t)0;                       /* unreachable, defensive */
    }
}
