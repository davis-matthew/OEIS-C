/*
 * A319722  Write n in 5-ary, sort digits into decreasing order.
 * https://oeis.org/A319722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number obtained by writing n in base‑5 and sorting the
 *            digits in descending order (then interpreting as base‑5)
 * Range:     u8 n <= 126, u16 n <= 3124, u32 n <= 3124, u64 n <= 3124,
 *            u128 n <= 3124, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    unsigned int count[5] = {0, 0, 0, 0, 0};

    /* Count base‑5 digits of n */
    idx_t tmp = n;
    if (tmp == 0) {
        count[0] = 1;
    } else {
        while (tmp > 0) {
            count[tmp % 5]++;   /* tmp and 5 are of type idx_t, % works */
            tmp /= 5;
        }
    }

    /* Build result with digits sorted from 4 down to 0 */
    for (int d = 4; d >= 0; --d) {
        for (unsigned int i = 0; i < count[d]; ++i) {
            result = result * 5 + d;
        }
    }

    return result;
}
