/*
 * A241496  Expansion of (1 + 4*x + x^2) / (1 - x^2)^3.
 * https://oeis.org/A241496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + ( 3*n*(n + 4) + 2 - (-1)^n*(n*(n + 4) + 2) ) / 8
 * Range:     u8 n <= 6, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;                 /* convert index to seq_t for arithmetic */
    seq_t t   = sn * (sn + 4);           /* t = n*(n+4) */

    seq_t part1 = 3 * t + 2;             /* 3*n*(n+4)+2 */
    seq_t part2 = t + 2;                 /*   n*(n+4)+2 */

    seq_t diff;
    if ((n & 1) == 0) /* n even -> (-1)^n = 1 */
        diff = part1 - part2;
    else              /* n odd  -> (-1)^n = -1 */
        diff = part1 + part2;

    seq_t result = 1 + diff / 8;
    return result;
}
