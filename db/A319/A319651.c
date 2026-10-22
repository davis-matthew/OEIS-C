/*
 * A319651  Largest number having in its ternary representation the same
 *          number of 0's, 1's and 2's as n.
 * https://oeis.org/A319651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = largest number whose base‑3 representation has the same *
 *            digit counts as that of n. The construction is simply * c2 times
 *            '2', then c1 times '1', then c0 times '0'
 * Range:     u8 n <= 243, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count digits of n in base 3 */
    idx_t c[3] = {0, 0, 0};

    if (n == 0) {
        /* representation of 0 is a single 0 digit */
        c[0] = 1;
    } else {
        idx_t t = n;
        while (t != 0) {
            idx_t d = t % 3;
            c[d] += 1;
            t /= 3;
        }
    }

    /* Build the largest number with the same counts */
    seq_t result = 0;
    for (int d = 2; d >= 0; --d) {
        for (idx_t i = 0; i < c[d]; ++i) {
            result = result * 3 + d;
        }
    }

    return result;
}
