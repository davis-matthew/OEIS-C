/*
 * A382574  a(n) is the number of shallow permutations of length n that avoid
 *          123.
 * https://oeis.org/A382574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 31, u64 n <= 65, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function
   (1-3*x+11*x^3-13*x^4+7*x^5+6*x^6+3*x^7) / ((1-x)^4 * (1-4*x^2+x^4)).

   The denominator yields the order‑8 linear recurrence
   a(n) = 4*a(n-1) - 2*a(n-2) - 12*a(n-3) + 22*a(n-4)
          - 12*a(n-5) - 2*a(n-6) + 4*a(n-7) - a(n-8)  (n ≥ 8)

   Initial values:
   a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=13, a(5)=35,
   a(6)=90, a(7)=225.  */

seq_t a(idx_t n)
{
    static const seq_t init[8] = {1ULL, 1ULL, 2ULL, 5ULL, 13ULL,
                                  35ULL, 90ULL, 225ULL};
    if (n < 8) {
        return init[n];
    }

    seq_t buf[8];
    /* buf[0] = a(n-1), buf[1] = a(n-2), …, buf[7] = a(n-8) */
    buf[0] = (seq_t)225;
    buf[1] = (seq_t)90;
    buf[2] = (seq_t)35;
    buf[3] = (seq_t)13;
    buf[4] = (seq_t)5;
    buf[5] = (seq_t)2;
    buf[6] = (seq_t)1;
    buf[7] = (seq_t)1;

    for (idx_t i = 8; i <= n; ++i) {
        seq_t next = (seq_t)4 * buf[0];
        next -= (seq_t)2 * buf[1];
        next -= (seq_t)12 * buf[2];
        next += (seq_t)22 * buf[3];
        next -= (seq_t)12 * buf[4];
        next -= (seq_t)2 * buf[5];
        next += (seq_t)4 * buf[6];
        next -= buf[7];

        /* shift buffer: a(n) becomes buf[0] for next iteration */
        for (int j = 7; j > 0; --j) {
            buf[j] = buf[j - 1];
        }
        buf[0] = next;
    }

    return buf[0];
}
