/*
 * A349278  a(n) is the product of the sum of the last i digits of n, with i
 *          going from 1 to the total number of digits of n.
 * https://oeis.org/A349278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product_{i=1}^{d} (sum of the last i digits of n) where d
 *            is the decimal digit length of n. a(n) = 0 iff n is a multiple
 *            of 10
 * Range:     u8 n <= 106, u16 n <= 1298, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {            /* sequence is defined for n≥1, but guard against 0 */
        return 0;
    }

    seq_t result = 1;        /* product accumulator */
    seq_t sum = 0;           /* running sum of trailing digits */

    idx_t tmp = n;           /* work with a copy of n */

    while (tmp > 0) {
        idx_t digit = tmp % 10;    /* least‑significant decimal digit */
        sum += (seq_t)digit;       /* update running sum with this digit */
        result *= sum;             /* multiply the current sum into the product */
        tmp /= 10;                 /* drop the processed digit */
    }
    return result;
}
