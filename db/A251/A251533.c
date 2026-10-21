/*
 * A251533  Clique covering number of obtuse knights graph N_n.
 * https://oeis.org/A251533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   For n <= 15 the values are given explicitly: a(1)=1, a(2)=3,
 *            a(3)=6, a(4)=7, a(5)=7, a(6)=9, a(7)=15, a(8)=18, a(9)=21,
 *            a(10)=21, a(11)=27, a(12)=31, a(13)=37, a(14)=42, a(15)=44. For
 *            n >= 16 the sequence satisfies the linear recurrence a(n) =
 *            a(n-1) + 7, i.e. a(n) = 7*n - 63. The function below...
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle indices below the offset – behaviour is unspecified, return 0 */
    if (n == 0) return (seq_t)0;

    /* Explicit values for n = 1..15 */
    if (n <= 15) {
        static const seq_t table[16] = {
            0,   /* placeholder for index 0 */
            1,   /* a(1) */
            3,   /* a(2) */
            6,   /* a(3) */
            7,   /* a(4) */
            7,   /* a(5) */
            9,   /* a(6) */
            15,  /* a(7) */
            18,  /* a(8) */
            21,  /* a(9) */
            21,  /* a(10) */
            27,  /* a(11) */
            31,  /* a(12) */
            37,  /* a(13) */
            42,  /* a(14) */
            44   /* a(15) */
        };
        return table[(unsigned)n];
    }

    /* For n >= 16: a(n) = 7*n - 63 */
    return (seq_t)7 * n - (seq_t)63;
}
