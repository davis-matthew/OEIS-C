/*
 * A328386  a(n) = A276086(n) mod n.
 * https://oeis.org/A328386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276086(n) mod n, where A276086(n) = Product_k
 *            prime(k+1)^d_k and d_k are the digits of n in primorial base;
 *            computed modulo n throughout
 * Range:     u8 n <= 269, u16 n <= 30030, u32 n <= 30030, u64 n <= 30030,
 *            u128 n <= 30030, big any n
 * Validated: standard, pass, 107 terms through n = 9838 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const uint64_t pr[20] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
    uint64_t m = (uint64_t)n;
    uint64_t r = 1 % m;
    uint64_t rem = m;
    int k = 0;
    while (rem > 0 && k < 20) {
        uint64_t p = pr[k];
        uint64_t d = rem % p;
        rem /= p;
        uint64_t pm = p % m;
        for (uint64_t i = 0; i < d; ++i) {
            r = (uint64_t)(((unsigned __int128)r * pm) % m);
        }
        ++k;
    }
    return (seq_t)r;
}
