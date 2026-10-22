/*
 * A374015  Residue modulo 5 of n! divided by the highest power of 10 which
 *          divides n!.
 * https://oeis.org/A374015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( n! / 10^min(v2(n!), v5(n!)) ) mod 5, where v2 and v5
 *            are exponents of 2 and 5 in n!
 * Range:     u8 n <= 9817, u16 n <= 9817, u32 n <= 9817, u64 n <= 9817,
 *            u128 n <= 9817, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the residue modulo 5 of n! after removing the largest power of 10 that divides n! */
seq_t a(idx_t n)
{
    if (n == 0) return 1;           /* 0! = 1, no 2 or 5 factors */

    seq_t prod = 1;                 /* product modulo 5 of numbers with all 2's and 5's removed */
    idx_t count2 = 0, count5 = 0;   /* total removed factors of 2 and 5 */

    for (idx_t i = 1; i <= n; ++i) {
        idx_t x = i;

        /* remove all 5 factors from x */
        while (x % 5 == 0) {
            x /= 5;
            ++count5;
        }

        /* remove all 2 factors from x */
        while (x % 2 == 0) {
            x /= 2;
            ++count2;
        }

        prod = (prod * (seq_t)(x % 5)) % 5;
    }

    /* We removed all 2's but need only to remove min(v2,v5) = count5 of them.
       Multiply back the remaining 2^(count2-count5) modulo 5.           */
    idx_t e = (count2 - count5) % 4;           /* exponent modulo 4 (period of 2 modulo 5) */
    static const seq_t pow2mod5[4] = {1, 2, 4, 3};

    return (prod * pow2mod5[e]) % 5;
}
