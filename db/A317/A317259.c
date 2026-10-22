/*
 * A317259  a(n) = 136926916457315893 + (n - 1)*9843204333812850.
 * https://oeis.org/A317259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 136926916457315893 + (n - 1)*9843204333812850
 * Range:     u8 none, u16 none, u32 none, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* offset starts at 1, return 0 for n<1 */

    const seq_t base = 136926916457315893ULL;
    const seq_t diff = 9843204333812850ULL;

    return base + ((seq_t)(n - 1)) * diff;
}
