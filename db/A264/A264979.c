/*
 * A264979  Bijective base-9 reverse: a(0) = 0; for n >= 1, a(n) =
 *          A030108(n/A264981(n)) * A264981(n).
 * https://oeis.org/A264979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = reverse base-9 digits of n (bijective base‑9 reverse)
 * Range:     u8 n <= 84, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* find the largest power of 9 dividing n */
    idx_t tmp = n;
    seq_t p = 1;
    while (tmp % 9 == 0) {
        p *= 9;
        tmp /= 9;
    }

    /* t = n / p  (now p is a power of 9 and divides n) */
    idx_t t = n / (idx_t)p;

    /* reverse t in base 9 */
    seq_t rev = 0;
    while (t > 0) {
        idx_t digit = t % 9;
        rev = rev * 9 + digit;
        t /= 9;
    }

    return rev * p;
}
