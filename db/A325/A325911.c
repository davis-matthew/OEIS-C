/*
 * A325911  Screaming numbers in base 16: numbers whose hexadecimal
 *          representation is AAAAAAA...
 * https://oeis.org/A325911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, dumb
 * Formula:   a(n) = 10*(16^n-1)/15; Equivalent recurrence: a(1)=10, a(2)=170,
 *            a(n)=17*a(n-1)-16*a(n-2) for n>2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)170;
    seq_t prev2 = (seq_t)10;   /* a(1) */
    seq_t prev1 = (seq_t)170;  /* a(2) */
    seq_t cur;
    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)17 * prev1 - (seq_t)16 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
