/*
 * A277568  Numbers k such that k/6^m == 2 (mod 6), where 6^m is the greatest
 *          power of 6 that divides k.
 * https://oeis.org/A277568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A277568
 * Formula: a(n) is the n-th positive integer k such that
 *          k / 6^m ≡ 2 (mod 6),
 *          where 6^m is the greatest power of 6 dividing k.
 *          Equivalently, in base‑6 the rightmost non‑zero digit is 2.
 *
 * This implementation enumerates the integers in increasing order
 * and selects those that satisfy the above condition.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1 – never called with 0 */

    idx_t count = 0;                /* how many qualifying numbers seen */
    seq_t val   = 0;                /* current integer being tested   */

    const seq_t six = 6;            /* used for the 6‑power test      */
    const seq_t two = 2;            /* target residue                */

    while (count < n)
    {
        val += 1;                   /* next integer to test          */
        seq_t tmp = val;

        /* strip off all factors of 6 */
        while ((tmp % six) == 0)
            tmp /= six;

        /* check the residue after removing 6‑powers */
        if ((tmp % six) == two)
            ++count;
    }

    return val;                     /* n-th qualifying integer        */
}
