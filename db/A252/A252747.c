/*
 * A252747  Numbers n such that the hexagonal number H(n) is equal to the sum
 *          of four consecutive squares.
 * https://oeis.org/A252747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+34*a(n-2)-34*a(n-3)-a(n-4)+a(n-5); initial values:
 *            a(1)=18, a(2)=42, a(3)=602, a(4)=1418, a(5)=20442
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed initial terms */
    static const seq_t init[5] = {
        (seq_t)18,
        (seq_t)42,
        (seq_t)602,
        (seq_t)1418,
        (seq_t)20442
    };

    if (n <= 5) {
        return init[n - 1];
    }

    /* Buffer to hold the last five values a(k-4)..a(k) */
    seq_t buf[5];
    for (int i = 0; i < 5; ++i) {
        buf[i] = init[i];
    }

    for (idx_t k = 6; k <= n; ++k) {
        seq_t new_val = buf[4] + (seq_t)34 * buf[3]
                       - (seq_t)34 * buf[2]
                       - buf[1]
                       + buf[0];

        /* Shift the buffer for next iteration */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = new_val;
    }

    return buf[4];
}
