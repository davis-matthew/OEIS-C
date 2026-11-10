/*
 * A397047  a(n) is the number of n-digit terms in A397046.
 * https://oeis.org/A397047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hard, more
 * Formula:   a(n) = number of terms of A397046 with exactly n digits; found
 *            by a sieve scanning k = 2, 3, ... (k is accepted if digsum(k) is
 *            divisible by no smaller accepted term), counting accepted terms
 *            by digit length
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t A397047_small[256];
static size_t A397047_nsmall = 0;
static uint64_t A397047_next = 2;
static uint64_t A397047_cnt[21];

seq_t a(idx_t n)
{
    if (n < 1 || n > 19) return (seq_t)0;
    uint64_t limit = 1;
    for (idx_t i = 0; i < n; i++) limit *= 10;
    while (A397047_next < limit) {
        uint64_t k = A397047_next++;
        uint64_t v = k, ds = 0;
        int len = 0;
        while (v > 0) { ds += v % 10; v /= 10; len++; }
        bool bad = false;
        for (size_t i = 0; i < A397047_nsmall && A397047_small[i] <= ds; ++i) {
            if (ds % A397047_small[i] == 0) { bad = true; break; }
        }
        if (!bad) {
            A397047_cnt[len]++;
            if (k <= 200 && A397047_nsmall < 256) A397047_small[A397047_nsmall++] = k;
        }
    }
    return (seq_t)A397047_cnt[n];
}
