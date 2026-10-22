/*
 * A397757  Maximum length of a snake polyomino in a 6 X n rectangle.
 * https://oeis.org/A397757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + floor((15*n + (n mod 4))/4) for n > 6. Recurrence:
 *            a(n) = a(n-4) + 15 for n >= 11. Base values: a(1)=6, a(2)=9,
 *            a(3)=14, a(4)=17, a(5)=21, a(6)=24, a(7)=29, a(8)=32, a(9)=36,
 *            a(10)=40
 * Range:     u8 n <= 16, u16 n <= 4649, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset and base cases */
    static const seq_t base[11] = {0, 6, 9, 14, 17, 21, 24, 29, 32, 36, 40};
    if (n <= 10)
    {
        if (n == 0) return (seq_t)0;
        return base[n];
    }

    /* For n > 10 use the closed‑form formula */
    seq_t t = (seq_t)15 * (seq_t)n + (seq_t)(n & 3); /* n mod 4 = n & 3 */
    return (seq_t)2 + (t / 4);
}
