/*
 * A346690  Replace 6^k with (-1)^k in base-6 expansion of n.
 * https://oeis.org/A346690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = Σ_{k≥0} (-1)^k * digit_k(n in base‑6)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = n + 7 * Σ_{k≥1} (-1)^k * ⌊n/6^k⌋ */
seq_t a(idx_t n) {
    idx_t nn = n;
    seq_t res = 0;
    seq_t sign = 1;
    while (nn > 0) {
        seq_t digit = (seq_t)(nn % 6);
        res += sign * digit;
        nn /= 6;
        sign = -sign;
    }
    return res;
}
