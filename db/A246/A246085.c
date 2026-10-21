/*
 * A246085  Paradigm shift sequence for (1,3) production scheme with
 *          replacement.
 * https://oeis.org/A246085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 89, u32 n <= 191, u64 n <= 393,
 *            u128 n <= 796, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3 * a(n-10) for all n >= 25.
   Initial values: a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=6,
                    a(7)=7, a(8)=8, a(9)=9, a(10)=10,
                    a(11)=11, a(12)=12, a(13)=13, a(14)=14,
                    a(15)=16, a(16)=18, a(17)=21, a(18)=24,
                    a(19)=27, a(20)=30, a(21)=33, a(22)=36,
                    a(23)=40, a(24)=44. */

seq_t a(idx_t n)
{
    /* Base values for 1 ≤ n ≤ 24 */
    static const seq_t base[24] = {
        1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
        11, 12, 13, 14,
        16, 18, 21, 24, 27, 30, 33, 36, 40, 44
    };

    if (n <= 24)
        return base[n - 1];

    seq_t prod = 1;
    idx_t m = n;
    while (m > 24) {
        prod *= 3;
        m -= 10;
    }
    return prod * base[m - 1];
}
