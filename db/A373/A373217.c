/*
 * A373217  Expansion of Sum_{k>=0} x^(7^k) / (1 - x^(7^k)).
 * https://oeis.org/A373217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = v_7(n)+1 (number of powers of 7 that divide n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety guard – not part of the sequence */
    idx_t t = n;
    seq_t res = 1;                 /* account for the 7⁰ factor */
    while (t % 7 == 0) {
        t /= 7;
        ++res;
    }
    return res;
}
