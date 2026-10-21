/*
 * A299189  Number of nX3 0..1 arrays with every element equal to 0, 1, 3, 4,
 *          6 or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + 13*a(n-3) - 13*a(n-4) - 46*a(n-5) -
 *            59*a(n-6) + 53*a(n-7) + 153*a(n-8) + 98*a(n-9) - 60*a(n-10) -
 *            140*a(n-11) - 7*a(n-12) + 67*a(n-13) + 15*a(n-14) - 78*a(n-15) -
 *            48*a(n-16) - 32*a(n-17) (for n > 18), with initial values
 *            a(1)=3, a(2)=3, a(3)=3, a(4)=9, a(5)=19, a(6)=59,...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[18] = {
        3, 3, 3, 9, 19, 59, 129, 355,
        891, 2317, 6019, 15543, 40557, 105639,
        274475, 716909, 1868955, 4870915
    };
    static const int coeff[17] = {
        1, 4, 13, -13, -46, -59, 53, 153, 98,
       -60, -140, -7, 67, 15, -78, -48, -32
    };

    if (n <= 1) {
        /* n is guaranteed >= 1 by the problem statement. */
        return init[0];
    }
    if (n <= 18) {
        return init[n - 1];
    }

    /* Keep the most recent 18 terms in a circular buffer. */
    seq_t buf[18];
    for (int i = 0; i < 18; ++i) {
        buf[i] = init[i];
    }

    /* Compute terms from 19 up to n. */
    for (idx_t k = 19; k <= n; ++k) {
        seq_t sum = 0;
        /* a(n-1) is stored at buf[17], a(n-2) at buf[16], ..., a(n-17) at buf[0] */
        for (int i = 0; i < 17; ++i) {
            sum += (seq_t)coeff[i] * buf[17 - i];
        }
        /* Shift the buffer left and insert the new term at position 17. */
        for (int i = 0; i < 17; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[17] = sum;
    }

    return buf[17];
}
