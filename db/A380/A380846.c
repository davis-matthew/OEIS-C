/*
 * A380846  Numbers k such that A380845(k) = 2*k.
 * https://oeis.org/A380846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th k such that A380845(k) = 2*k, where A380845(k) =
 *            Sum_{d|k} d*[popcount(d) = popcount(k)]; computed by a memoized
 *            search over k
 * Range:     u8 n <= 5, u16 n <= 803, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9040 (2026-11-10)
 * Helpers:   factor_u64, popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/popcount.h"

static void P_add_divs(const uint64_t *pr, const uint64_t *ex, int depth, int total,
                       uint64_t cur, int target_pc, uint64_t *sum)
{
    if (depth == total) {
        if (popcount(cur) == target_pc) *sum += cur;
        return;
    }
    uint64_t val = cur;
    for (uint64_t e = 0; e <= ex[depth]; ++e) {
        P_add_divs(pr, ex, depth + 1, total, val, target_pc, sum);
        val *= pr[depth];
    }
}

static uint64_t P_A380845(uint64_t k)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    uint64_t sum = 0;
    factor_u64(k, pr, ex, &cnt);
    P_add_divs(pr, ex, 0, (int)cnt, 1ULL, popcount(k), &sum);
    return sum;
}

seq_t a(idx_t n)
{
    static uint64_t terms[100000];
    static idx_t found = 0;
    static uint64_t next_k = 1;

    if (n < 1 || n > 100000) return 0;
    while (found < n) {
        uint64_t k = next_k++;
        if (P_A380845(k) == 2 * k) terms[found++] = k;
    }
    return (seq_t)terms[n - 1];
}
