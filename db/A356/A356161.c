/*
 * A356161  a(n) = 1 if the smallest positive k such that n divides
 *          k*A276086(k) is equal to n, otherwise 0, where A276086 is
 *          primorial base exp-function.
 * https://oeis.org/A356161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [A355944(n) == n], computed by search: a(n)=1 iff no k<n
 *            has n | k*A276086(k)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 9435 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static const uint64_t pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    uint64_t m = (uint64_t)n;
    if (m <= 1) return (seq_t)1;
    for (uint64_t k = 1; k < m; ++k) {
        uint64_t acc = 1 % m;
        uint64_t x = k;
        int i = 0;
        while (x > 0) {
            uint64_t d = x % pr[i];
            x /= pr[i];
            uint64_t p = pr[i] % m;
            while (d--) acc = (acc * p) % m;
            ++i;
        }
        if (((k % m) * acc) % m == 0) return (seq_t)0;
    }
    return (seq_t)1;
}
