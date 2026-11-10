/*
 * A375429  The indices of the terms of A375428 in the Fibonacci sequence.
 * https://oeis.org/A375429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A130233(A051903(n)); i.e. the largest k with Fibonacci(k)
 *            <= (max exponent in the prime factorization of n), a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 8957 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    uint64_t e = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] > e) e = ex[i];

    /* largest k with F(k) <= e, F(1)=F(2)=1, index of 1 chosen as 2 */
    uint64_t prev = 1, cur = 2;   /* F(2), F(3) */
    unsigned k = 2;
    while (cur <= e) {
        uint64_t next = prev + cur;
        prev = cur;
        cur = next;
        ++k;
    }
    return (seq_t)k;
}
