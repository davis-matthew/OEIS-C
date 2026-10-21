/*
 * A265207  Draw a square and follow these steps: Take a square and place at
 *          its edges isosceles right triangles with the edge as hypotenuse.
 *          Draw a square at every new edge of the triangles. Repeat for all
 *          the new squares of the same size. New figures are only placed on
 *          empty space. The structure is symmetr...
 * https://oeis.org/A265207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=8, a(3)=20 for n≥4: a(n)=2*a(n-2)+20 (i.e. even and
 *            odd indices evolve independently with the same recurrence)
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset 1 – the sequence is undefined for n<1 */
    if (n < 1)
        return 0;

    /* base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)20;

    /* compute by two independent recurrences for even and odd indices */
    if ((n & 1) == 0) {          /* even n: a(2)=8, a(4)=36, ... */
        seq_t val = (seq_t)8;
        for (idx_t k = 4; k <= n; k += 2) {
            val = (seq_t)2 * val + (seq_t)20;
        }
        return val;
    } else {                     /* odd n: a(3)=20, a(5)=60, ... */
        seq_t val = (seq_t)20;
        for (idx_t k = 5; k <= n; k += 2) {
            val = (seq_t)2 * val + (seq_t)20;
        }
        return val;
    }
}
