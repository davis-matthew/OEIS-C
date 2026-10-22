/*
 * A347558  Number of minimum dominating sets in the n-ladder graph.
 * https://oeis.org/A347558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 for odd n ≥ 1, except a(3) = 3 a(n) = 2*n + 4 = 2*(n+2)
 *            for even n ≥ 8 Special cases: a(1)=2, a(2)=6, a(3)=3, a(4)=12,
 *            a(5)=2, a(6)=17 This matches the recurrence a(n)=2*a(n-2)-a(n-4)
 *            for n>6 while also respecting the anomalies at n=2 and n=6. The
 *            implementation uses only integer arithme...
 * Range:     u8 n <= 125, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first few terms explicitly */
    if (n <= 6) {
        switch ((int)n) {
            case 1: return (seq_t)2;
            case 2: return (seq_t)6;
            case 3: return (seq_t)3;
            case 4: return (seq_t)12;
            case 5: return (seq_t)2;
            case 6: return (seq_t)17;
        }
    }

    /* For odd indices (n ≥ 7) the value is always 2 */
    if ((n & (idx_t)1) != 0) {
        return (seq_t)2;
    }

    /* For even indices (n ≥ 8) use the closed formula 2*(n+2) = 2n+4 */
    return ((seq_t)n) * (seq_t)2 + (seq_t)4;
}
