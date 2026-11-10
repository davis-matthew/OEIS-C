/*
 * A381489  Index of first half of decomposition of integers into pairs
 *          x(i)+y(j) based on A380008 and A380009, respectively.
 * https://oeis.org/A381489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k>=0, A000069(k)=e} bit e of n * 2^k, where e runs
 *            over odious numbers (A000069) in increasing order and k is the
 *            rank of e; this inverts n = A380008(a(n)) + A380009(A381490(n))
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Validated: partial, pass, 96 terms through n = 95 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 0;
    unsigned i = 0; /* rank among odious numbers */
    unsigned e;
    for (e = 0; e < 64; ++e) {
        if (__builtin_popcountll((unsigned long long)e) & 1) {
            if ((n >> e) & 1U) {
                result |= ((seq_t)1 << i);
            }
            ++i;
        }
    }
    return result;
}
