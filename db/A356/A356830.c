/*
 * A356830  Number of vertex cuts in the n-prism graph.
 * https://oeis.org/A356830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10*a(n-1) - 35*a(n-2) + 48*a(n-3)
 *            - 11*a(n-4) - 22*a(n-5) + 7*a(n-6) + 4*a(n-7)
 * Initial values: a(1)=0, a(2)=2, a(3)=12, a(4)=88, a(5)=520,
 *                 a(6)=2654, a(7)=12376
 */
seq_t a(idx_t n)
{
    if (n <= 7) {
        static const seq_t init[7] = {0, 2, 12, 88, 520, 2654, 12376};
        return init[n - 1];
    }

    seq_t buf[7];
    buf[0] = 0;       // a(1)
    buf[1] = 2;       // a(2)
    buf[2] = 12;      // a(3)
    buf[3] = 88;      // a(4)
    buf[4] = 520;     // a(5)
    buf[5] = 2654;    // a(6)
    buf[6] = 12376;   // a(7)

    for (idx_t i = 8; i <= n; ++i) {
        seq_t pos = 10 * buf[(i - 2) % 7]          // a(i-1)
                 + 48 * buf[(i - 4) % 7]          // a(i-3)
                 +  7 * buf[(i - 7) % 7]          // a(i-6)
                 +  4 * buf[(i - 8) % 7];         // a(i-7)

        seq_t neg = 35 * buf[(i - 3) % 7]          // a(i-2)
                 + 11 * buf[(i - 5) % 7]          // a(i-4)
                 + 22 * buf[(i - 6) % 7];         // a(i-5)

        buf[(i - 1) % 7] = pos - neg;
    }

    return buf[(n - 1) % 7];
}
