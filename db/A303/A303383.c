/*
 * A303383  Total volume of all cubes with side length q such that n = p + q
 *          and p <= q.
 * https://oeis.org/A303383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=1..floor(n/2)} (n-i)^3 Equivalent: if n is even,
 *            a(n) = n^2 * (3n-2) * (5n-6) / 64; if n is odd, a(n) = (n-1)^2 *
 *            (3n-1) * (5n+1) / 64
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not used in offset 1 sequence */
    seq_t nn = (seq_t)n;

    if ((nn & 1) == 0) {           /* even n */
        seq_t t = nn * nn;                 /* n^2 */
        t = t * (3 * nn - 2);              /* n^2 * (3n-2) */
        t = t * (5 * nn - 6);              /* * (5n-6) */
        return t / 64;                     /* divide by 64 */
    } else {                        /* odd n */
        seq_t n1 = nn - 1;
        seq_t t = n1 * n1;                 /* (n-1)^2 */
        t = t * (3 * nn - 1);              /* * (3n-1) */
        t = t * (5 * nn + 1);              /* * (5n+1) */
        return t / 64;
    }
}
