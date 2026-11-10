/*
 * A335420  a(n) = A000120(A163511(n)).
 * https://oeis.org/A335420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000120(A163511(n)); A163511(n) = p(r) * Prod_{i=1..r}
 *            p(i)^(e_i - e_{i-1} - 1), where e_i are the 1-based positions of
 *            the set bits of n and r = A000120(n); a(0) = 1
 * Range:     u8 n <= 34, u16 n <= 1091, u32 n <= 8236, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9315 (2026-11-08)
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/nth_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t pos[64];
    idx_t cnt = 0;
    idx_t bitpos = 1;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1) pos[cnt++] = bitpos;
        ++bitpos;
        tmp >>= 1;
    }

    seq_t result = (seq_t)nth_prime((idx_t)cnt);
    idx_t prev = 0;
    for (idx_t i = 0; i < cnt; ++i) {
        idx_t e = pos[i] - prev - 1;
        seq_t base = (seq_t)nth_prime(i + 1);
        for (idx_t k = 0; k < e; ++k) result *= base;
        prev = pos[i];
    }

    seq_t c = 0;
    while (result != 0) {
        c += (result & 1);
        result >>= 1;
    }
    return c;
}
