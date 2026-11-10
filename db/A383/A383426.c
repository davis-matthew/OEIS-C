/*
 * A383426  Indices k such that A003849(k) = 1 and A383422(k) = 0.
 * https://oeis.org/A383426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) is the n-th index k >= 0 where A003849(k) = 1 and
 *            A383422(k) = 0
 * Range:     u8 n <= 38, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9877 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A383426_word(uint64_t n, uint64_t l0, uint64_t l1)
{
    uint64_t len[100];
    unsigned k;
    len[0] = l0;
    len[1] = l1;
    k = 1;
    while (len[k] <= n) {
        ++k;
        len[k] = len[k - 1] + len[k - 2];
    }
    while (k > 1) {
        if (n < len[k - 1]) {
            --k;
        } else {
            n -= len[k - 1];
            k -= 2;
        }
    }
    if (k == 0) return 0;
    return n == 0 ? 0 : 1;
}

static int A383426_match(uint64_t pos)
{
    return A383426_word(pos, 1, 2) == 1 && A383426_word(pos, 1, 3) == 0;
}

seq_t a(idx_t n)
{
    static uint64_t cache_cnt = 0;
    static uint64_t cache_pos = 0;
    static int have = 0;
    uint64_t cnt, pos;
    if (have && cache_cnt == (uint64_t)n) return (seq_t)cache_pos;
    if (have && cache_cnt < (uint64_t)n) {
        cnt = cache_cnt;
        pos = cache_pos + 1;
    } else {
        cnt = 0;
        pos = 0;
    }
    for (;; ++pos) {
        if (A383426_match(pos)) {
            ++cnt;
            if (cnt == (uint64_t)n) {
                cache_cnt = cnt;
                cache_pos = pos;
                have = 1;
                return (seq_t)pos;
            }
        }
    }
}
