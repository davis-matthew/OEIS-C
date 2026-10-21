/*
 * A297609  Number of 3Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 1 or 3 neighboring 1s.
 * https://oeis.org/A297609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)
 *            +7*a(n-2)+13*a(n-3)+52*a(n-4)+19*a(n-5)-36*a(n-6)+2*a(n-7)-98*a(n-8)-31*a(n-9)+32*a(n-10)+22*a(n-11)+6*a(n-12)+a(n-13)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values (n = 1..13) are listed in OEIS A297609 */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[13] = {
        (seq_t)8,         (seq_t)35,        (seq_t)181,       (seq_t)787,
        (seq_t)3691,      (seq_t)17186,     (seq_t)80140,     (seq_t)371557,
        (seq_t)1727168,   (seq_t)8029262,   (seq_t)37322914,  (seq_t)173454722,
        (seq_t)806192395
    };

    if (n <= 13)
        return init[n - 1];

    /* Circular buffer holding a(k-13) … a(k) */
    seq_t buf[13];
    for (size_t i = 0; i < 13; ++i)
        buf[i] = init[i];

    for (idx_t k = 14; k <= n; ++k) {
        seq_t next =
            2 * buf[12] + 7 * buf[11] + 13 * buf[10] + 52 * buf[9] + 19 * buf[8]
            - 36 * buf[7] + 2 * buf[6] - 98 * buf[5] - 31 * buf[4]
            + 32 * buf[3] + 22 * buf[2] + 6 * buf[1] + buf[0];

        /* Shift buffer left: buf[0] becomes buf[1], …, buf[12] becomes next */
        for (size_t j = 0; j < 12; ++j)
            buf[j] = buf[j + 1];
        buf[12] = next;
    }

    return buf[12];
}
