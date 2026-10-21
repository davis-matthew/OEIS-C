/*
 * A288937  Positions of 0 in A288936; complement of A288938.
 * https://oeis.org/A288937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=4, a(4)=5, a(5)=8, a(6)=11; For n >= 7,
 *            a(n) = n*(n+1)/2 - 3*n + 8
 * Range:     u8 n <= 24, u16 n <= 364, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed values for the first six indices */
    static const seq_t first[] = { 0, /* placeholder for 0 */
                                   1, 2, 4, 5, 8, 11 };

    if (n < 7) {
        return first[n];
    }

    seq_t tmp = (seq_t)n * ((seq_t)n + 1) / 2;  /* n*(n+1)/2  */
    tmp -= 3 * (seq_t)n;                        /* subtract 3*n */
    tmp += 8;                                   /* add constant 8 */

    return tmp;
}
