/*
 * A358846  a(n) = 1 if A276086(6*n) == 5 (mod 6), otherwise 0, where A276086
 *          is the primorial base exp-function.
 * https://oeis.org/A358846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(n) = a(n-1) XOR A358847(n), where A358847(k) = 1 if
 *            the smallest prime >= 5 not dividing k is == 5 (mod 6), else 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 9616 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static const uint32_t pr[] = {5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
    const size_t cnt = sizeof(pr) / sizeof(pr[0]);
    unsigned acc = 0;
    for (uint64_t k = 1; k <= (uint64_t)n; ++k) {
        unsigned bit = 0;
        for (size_t i = 0; i < cnt; ++i) {
            if (k % pr[i] != 0) {
                bit = (pr[i] % 6 == 5) ? 1u : 0u;
                break;
            }
        }
        acc ^= bit;
    }
    return (seq_t)acc;
}
