/*
 * A356964  Replace 2^k in binary expansion of n with tribonacci(k+3) (where
 *          tribonacci corresponds to A000073).
 * https://oeis.org/A356964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{k≥0, (n>>k)&1} tribonacci(k+3) where tribonacci(0)=0,
 *            tribonacci(1)=0, tribonacci(2)=1,
 *            tribonacci(m)=tribonacci(m-1)+tribonacci(m-2)+tribonacci(m-3)
 * Range:     u8 n <= 413, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* tribonacci values up to index 64+3 = 67 are sufficient for uint64_t indices */
    enum { MAX_T_BITS = 64, MAX_T_INDEX = MAX_T_BITS + 3 };   /* 67 */
    static seq_t trib[MAX_T_INDEX + 1];
    static bool init = false;

    if (!init) {
        trib[0] = 0;
        trib[1] = 0;
        trib[2] = 1;
        for (idx_t i = 3; i <= MAX_T_INDEX; ++i) {
            trib[i] = trib[i - 1] + trib[i - 2] + trib[i - 3];
        }
        init = true;
    }

    seq_t sum = 0;
    idx_t bit = 0;
    while (n != 0) {
        if ((n & 1) != 0) {
            sum += trib[bit + 3];
        }
        n >>= 1;
        ++bit;
    }
    return sum;
}
