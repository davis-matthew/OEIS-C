/*
 * A385029  a(n) = Sum_{-n <= a, b, c <= n} (b^2 - 4*a*c).
 * https://oeis.org/A385029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n+1)^3/3
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 68, u64 n <= 5860,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a = (seq_t)n;
    seq_t b = (seq_t)(n + 1);
    seq_t c = (seq_t)(2 * (seq_t)n + 1);

    seq_t prod = a;
    prod *= b;           /* prod = a*b            */
    prod *= c;           /* prod = a*b*c          */
    prod /= 3;           /* divide by 3            */
    prod *= c;           /* prod = a*b*c^2 / 3    */
    prod *= c;           /* prod = a*b*c^3 / 3    */
    return prod;
}
