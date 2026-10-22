/*
 * A319654  Write n in 6-ary, sort digits into increasing order.
 * https://oeis.org/A319654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = interpret(sorted base‑6 digits of n as base‑6 number)
 * Range:     u8 n <= 258, u16 n <= 7775, u32 n <= 7775, u64 n <= 7775,
 *            u128 n <= 7775, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Equivalent: write n in base‑6, sort its digits in non‑decreasing order,
 *   then read the sorted string as a base‑6 integer.            */

seq_t a(idx_t n)
{
    /* Count digits 0–5 in the base‑6 representation of n */
    unsigned char cnt[6] = {0};
    if (n == 0) {
        cnt[0] = 1;
    } else {
        idx_t t = n;
        while (t > 0) {
            unsigned d = t % 6;   /* digit 0…5 */
            cnt[d]++;
            t /= 6;
        }
    }

    /* Reconstruct the number from the sorted digits (ascending) */
    seq_t res = 0;
    for (unsigned d = 0; d < 6; d++) {
        unsigned c = cnt[d];
        while (c--) {
            res = res * 6 + d;
        }
    }
    return res;
}
