/*
 * A264797  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 342", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A264797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) - 2*a(n-6) + a(n-8) for n ≥ 8 * Initial values:
 *            a(0)=1, a(1)=5, a(2)=8, a(3)=24, a(4)=17, * a(5)=53, a(6)=32,
 *            a(7)=96
 * Range:     u8 n <= 12, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for indices 0 … 7 */
    static const seq_t base[8] = {
        (seq_t)1, (seq_t)5, (seq_t)8, (seq_t)24,
        (seq_t)17, (seq_t)53, (seq_t)32, (seq_t)96
    };

    if (n < 8u) {
        return base[(size_t)n];
    }

    /* Circular buffer holding the last eight values.
     * index 0 corresponds to a(i-8), 1 to a(i-7), …, 7 to a(i-1) */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i) {
        buf[i] = base[i];
    }

    for (idx_t i = 8; i <= n; ++i) {
        /* Calculate a(i) using the recurrence */
        seq_t val = 2u * buf[(i - 2) & 7u]   /* a(i-2) */
                  - 2u * buf[(i - 6) & 7u]   /* a(i-6) */
                  +     buf[(i - 8) & 7u];   /* a(i-8) */

        buf[i & 7u] = val;  /* overwrite the oldest value */
    }

    return buf[n & 7u];
}
