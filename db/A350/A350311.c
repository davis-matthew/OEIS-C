/*
 * A350311  Replace 2^k in the binary expansion of n with A000930(k+2),
 *          Narayana's cows sequence.
 * https://oeis.org/A350311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k} A000930(k+4) where the sum is over all k with * the
 *            k-th bit of n (0‑based) equal to 1. A000930 is Narayana’s cows *
 *            sequence defined by * a(0)=0, a(1)=0, a(2)=1, a(n)=a(n-1)+a(n-3)
 *            (n≥3)
 * Range:     u8 n <= 4021, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t nar[70];
    static bool init = false;
    if (!init) {
        nar[0] = 0;
        nar[1] = 0;
        nar[2] = 1;
        for (idx_t i = 3; i < 70; ++i)
            nar[i] = nar[i - 1] + nar[i - 3];
        init = true;
    }

    seq_t sum = 0;
    idx_t bit = 0;
    idx_t m = n;
    while (m) {
        if (m & 1)
            sum += nar[bit + 4];
        ++bit;
        m >>= 1;
    }
    return sum;
}
