/*
 * A319723  Write n in 6-ary, sort digits into decreasing order.
 * https://oeis.org/A319723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (write n in base 6, sort its digits in decreasing order,
 *            interpret the result as a base‑6 number)
 * Range:     u8 n <= 217, u16 n <= 7775, u32 n <= 7775, u64 n <= 7775,
 *            u128 n <= 7775, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count digits 0–5 in the base‑6 expansion of n */
    unsigned int counts[6] = {0};
    idx_t temp = n;
    while (temp != 0) {
        unsigned int digit = (unsigned int)(temp % 6);
        ++counts[digit];
        temp /= 6;
    }

    /* Reconstruct the number from the sorted digits (descending order) */
    seq_t result = 0;
    for (int d = 5; d >= 0; --d) {
        for (unsigned int i = 0; i < counts[d]; ++i) {
            result = result * 6 + (seq_t)d;
        }
    }

    return result;
}
