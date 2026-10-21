/*
 * A283586  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 654", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-8) - 10*a(n-9) for n > 12, with a(0)=1,
 *            a(1)=11, a(2)=111, a(3)=1011, a(4)=10111, a(5)=101011,
 *            a(6)=1000111, a(7)=11011011, a(8)=110110111, a(9)=1101101011,
 *            a(10)=11011000111, a(11)=110111011011, a(12)=1101010110111
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Initialize the constant values using seq_from_str. */
    static bool init_done = false;
    static seq_t init_lut[13];

    if (!init_done) {
        init_lut[0]  = seq_from_str("1");
        init_lut[1]  = seq_from_str("11");
        init_lut[2]  = seq_from_str("111");
        init_lut[3]  = seq_from_str("1011");
        init_lut[4]  = seq_from_str("10111");
        init_lut[5]  = seq_from_str("101011");
        init_lut[6]  = seq_from_str("1000111");
        init_lut[7]  = seq_from_str("11011011");
        init_lut[8]  = seq_from_str("110110111");
        init_lut[9]  = seq_from_str("1101101011");
        init_lut[10] = seq_from_str("11011000111");
        init_lut[11] = seq_from_str("110111011011");
        init_lut[12] = seq_from_str("1101010110111");
        init_done = true;
    }

    if (n <= 12u) {
        return init_lut[(size_t)n];
    }

    /* Extend the values iteratively. */
    seq_t buf[13];
    for (size_t i = 0; i < 13; ++i) {
        buf[i] = init_lut[i];
    }

    for (idx_t i = 13; i <= n; ++i) {
        size_t base = (size_t)(i % 13u);
        size_t idx1 = (base + 12) % 13;   /* i-1  */
        size_t idx8 = (base + 5)  % 13;   /* i-8  */
        size_t idx9 = (base + 4)  % 13;   /* i-9  */

        seq_t val = buf[idx1] * (seq_t)10
                    + buf[idx8]
                    - buf[idx9] * (seq_t)10;

        buf[base] = val;
    }

    return buf[(size_t)(n % 13u)];
}
