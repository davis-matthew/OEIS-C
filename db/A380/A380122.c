/*
 * A380122  a(n) is the number of integers m (possibly negative) such that the
 *          nonzero digits in the nonadjacent form for m appear in the
 *          nonadjacent form for n.
 * https://oeis.org/A380122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2^w, where w is the number of non‑zero digits in the
 *            non‑adjacent form (NAF) of n
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t w = 0;                /* number of NAF non‑zero digits */
    while (n) {
        if (n & 1) {            /* odd -> non‑zero digit in NAF */
            w++;
            /* choose NAF digit u = 1 or –1 */
            if ((n & 3) == 1)   /* n mod 4 == 1  →  u = +1  */
                n = (n - 1) >> 1;
            else                /* n mod 4 == 3  →  u = –1  */
                n = (n + 1) >> 1;
        } else {
            n >>= 1;             /* even digit, shift */
        }
    }
    /* compute 2^w in seq_t */
    seq_t result = 1;
    for (idx_t i = 0; i < w; ++i)
        result <<= 1;
    return result;
}
