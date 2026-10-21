/*
 * A290470  Number of minimal edge covers in the n-Moebius ladder.
 * https://oeis.org/A290470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=a(n-1)+2*a(n-2)+6*a(n-3)+2*a(n-4)+2*a(n-5)-2*a(n-6)-2*a(n-7)-a(n-8)+a(n-9)
 *            * Initial values: a(1)=3, a(2)=7, a(3)=15, a(4)=59, a(5)=143,
 *            a(6)=367, a(7)=1039, * a(8)=2755, a(9)=7395
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[10] = {
    (seq_t)0, /* unused */
    (seq_t)3, /* a(1) */
    (seq_t)7, /* a(2) */
    (seq_t)15,/* a(3) */
    (seq_t)59,/* a(4) */
    (seq_t)143,/* a(5) */
    (seq_t)367,/* a(6) */
    (seq_t)1039,/* a(7) */
    (seq_t)2755,/* a(8) */
    (seq_t)7395/* a(9) */
};

seq_t a(idx_t n)
{
    if (n <= 9u) {
        return init[(size_t)n];
    }

    seq_t buf[9];
    for (size_t i = 0; i < 9; ++i) {
        buf[i] = init[i + 1];
    }

    seq_t value = 0;
    for (idx_t current = 10u; current <= n; ++current) {
        value = buf[8];                                     /* a(n-1) */
        value += (seq_t)2 * buf[7];                         /* + 2*a(n-2) */
        value += (seq_t)6 * buf[6];                         /* + 6*a(n-3) */
        value += (seq_t)2 * buf[5];                         /* + 2*a(n-4) */
        value += (seq_t)2 * buf[4];                         /* + 2*a(n-5) */
        value -= (seq_t)2 * buf[3];                         /* - 2*a(n-6) */
        value -= (seq_t)2 * buf[2];                         /* - 2*a(n-7) */
        value -= buf[1];                                    /* - a(n-8) */
        value += buf[0];                                    /* + a(n-9) */

        for (size_t j = 0; j < 8; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[8] = value;
    }

    return value;
}
