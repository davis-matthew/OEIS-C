/*
 * A328624  Twisted variant of A276086 indexed by A328625.
 * https://oeis.org/A328624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276086(A328625(n)). Write n in primorial base with
 *            digits d_k (radix prime(k+1)); the new digits are e_0 = d_0 and
 *            e_k = d_k*(1+d_{k-1}) mod prime(k+1). Then a(n) = Product_k
 *            prime(k+1)^e_k
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 266, u64 n <= 1544,
 *            u128 n <= 2310, big any n
 * Validated: standard, pass, 78 terms through n = 2022 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint32_t A328624_pr[20] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};

seq_t a(idx_t n)
{
    uint64_t d[20];
    size_t len = 0;
    uint64_t v = (uint64_t)n;
    while (v != 0 && len < 20) {
        d[len] = v % A328624_pr[len];
        v /= A328624_pr[len];
        ++len;
    }
    seq_t result = 1;
    for (size_t k = 0; k < len; ++k) {
        uint64_t e = (k == 0) ? d[0] : (d[k] * (d[k - 1] + 1)) % A328624_pr[k];
        for (uint64_t i = 0; i < e; ++i)
            result = result * (seq_t)A328624_pr[k];
    }
    return result;
}
