/*
 * A256313  Number of partitions of 3n into exactly 4 parts.
 * https://oeis.org/A256313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if (n%2==0) { if ((n/2)%2==0) a = (6*n^3 + 6*n^2) / 32;
 *            else a = (6*n^3 + 6*n^2 - 8) / 32; } else { a = (6*n^3 + 6*n^2 -
 *            6*n - 6) / 32; } (Derived from the OEIS closed form.)
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 983, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* cast n to the sequence type for arithmetic */
    seq_t ns = (seq_t)n;
    seq_t nsq = ns * ns;        /* n^2 */
    seq_t ncu = nsq * ns;       /* n^3 */

    seq_t t6nc = (seq_t)6 * ncu;    /* 6*n^3 */
    seq_t t6ns = (seq_t)6 * nsq;    /* 6*n^2 */
    seq_t numerator;

    if ((n & (idx_t)1) == 0) {                /* n even */
        if (((n >> 1) & (idx_t)1) == 0) {     /* (n/2) even => n%4==0 */
            numerator = t6nc + t6ns;
        } else {                              /* (n/2) odd => n%4==2 */
            numerator = t6nc + t6ns - (seq_t)8;
        }
    } else {                                  /* n odd */
        numerator = t6nc + t6ns
                      - (seq_t)6 * ns
                      - (seq_t)6;
    }

    return numerator / (seq_t)32;
}
