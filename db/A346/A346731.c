/*
 * A346731  Replace 8^k with (-1)^k in base-8 expansion of n.
 * https://oeis.org/A346731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Σ_{k≥0} (-1)^k * (n / 8^k mod 8) (alternating sum of the
 *            octal digits of n)
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = 0;
    int sgn = 1;                     /* current sign (+1 or -1) */
    idx_t m = n;                      /* working copy of n */
    while (m) {
        /* extract least‑significant octal digit */
        res += (seq_t)sgn * (seq_t)(m & 7);
        /* shift right by 3 bits → divide by 8 */
        m >>= 3;
        sgn = -sgn;                  /* flip sign for next digit */
    }
    return res;
}
