/*
 * A267057  Binary representation of the n-th iteration of the "Rule 97"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 100000000*a(n-4) - 100000000*a(n-6) (for n>5)
 *            Initial values: a(0)=1, a(1)=0, a(2)=10100, a(3)=10011,
 *            a(4)=100000000, a(5)=11111111
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* initial terms */
    static const seq_t init[6] = {
        (seq_t)1,
        (seq_t)0,
        (seq_t)10100,
        (seq_t)10011,
        (seq_t)100000000,
        (seq_t)11111111
    };

    if (n < 6)
        return init[n];

    /* circular buffer for the last 7 computed values */
    seq_t buf[7];
    for (int i = 0; i < 6; ++i)
        buf[i] = init[i];

    const seq_t MUL = (seq_t)100000000;
    for (idx_t i = 6; i <= n; ++i) {
        seq_t val = buf[(i-2) % 7] + MUL * buf[(i-4) % 7];
        val -= MUL * buf[(i-6) % 7];
        buf[i % 7] = val;
    }
    return buf[n % 7];
}
