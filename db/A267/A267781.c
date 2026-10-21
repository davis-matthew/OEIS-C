/*
 * A267781  Expansion of (x*(-14x^6 - 32x^5 + 16x^4 + 30x^3 - x + 14) +
 *          17)/((x-1)^2*(x+1)).
 * https://oeis.org/A267781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (30*n + (-1)^n + 93)/2 for n >= 6. Small n (1..5) are
 *            provided explicitly
 * Range:     u8 n <= 5, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* handle the initial 5 terms explicitly */
    if (n == 1) return 17;
    if (n == 2) return 31;
    if (n == 3) return 47;
    if (n == 4) return 61;
    if (n == 5) return 107;

    /* For n >= 6: a(n) = (30*n + (-1)^n + 93)/2 */
    seq_t term = (seq_t)30 * n + 93;

    /* (-1)^n is +1 for even n, -1 for odd n. */
    if (n & 1)          /* odd n: subtract 1 */
        term -= 1;
    else                /* even n: add 1 */
        term += 1;

    return term / 2;
}
