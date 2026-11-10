/*
 * A245612  Permutation of natural numbers: a(0) = 1, a(1) = 2, a(2n) =
 *          3*a(n)-1, a(2n+1) = A254049(a(n)); composition of A048673 and
 *          A163511.
 * https://oeis.org/A245612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A048673(A163511(n)); A163511(n) = p(r)*Prod_{i=1..r}
 *            p(i)^(e_i) with r = popcount(n), e_i = gap before i-th set bit
 *            of n; A048673 shifts each prime to the next prime, then (x+1)/2.
 *            a(0)=1, a(1)=2
 * Range:     u8 n <= 16, u16 n <= 134, u32 n <= 4102, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 80 terms through n = 7762 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint32_t A245612_prime(int k) /* k-th prime, k>=1 */
{
    uint32_t cnt = 0, c = 1;
    while (cnt < (uint32_t)k) {
        ++c;
        bool ok = true;
        for (uint32_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) ++cnt;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    uint32_t pr[70];
    for (int i = 1; i <= 66; ++i) pr[i] = A245612_prime(i);

    int r = 0;
    idx_t prev = 0, bitpos = 1, tmp = n;
    seq_t prod = (seq_t)1;
    while (tmp) {
        if (tmp & 1) {
            ++r;
            idx_t e = bitpos - prev - 1;
            seq_t base = (seq_t)pr[r + 1]; /* shifted p(r) */
            for (idx_t k = 0; k < e; ++k) prod = prod * base;
            prev = bitpos;
        }
        ++bitpos;
        tmp >>= 1;
    }
    prod = prod * (seq_t)pr[r + 1];
    return (prod + (seq_t)1) / (seq_t)2;
}
