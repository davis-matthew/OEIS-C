/*
 * A394572  Minimal value of Sum_{i=1..n} p(i)*p((i mod n)+1), as p ranges
 *          over all permutations of powers of 2 {1,2,4,...,2^(n-1)}.
 * https://oeis.org/A394572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 8*a(n-3) for n > 3, with a(0) = 0,
 *            a(1) = 1, a(2) = 4, a(3) = 14
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)4;
    if (n == 3)
        return (seq_t)14;

    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)4;   /* a(n-2) */
    seq_t a3 = (seq_t)14;  /* a(n-1) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = 2 * a3 + 4 * a2 - 8 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
