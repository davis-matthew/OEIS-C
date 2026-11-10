/*
 * A389106  Least positive integer k such that A386395(k*n) has the least
 *          possible number of digits.
 * https://oeis.org/A389106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k >= 1 such that A386395(k*n) has the fewest
 *            digits, i.e. fewer than 10 if n is not a multiple of 10, else
 *            fewer than 100 (search over k)
 * Range:     u8 n <= 18, u16 n <= 718, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 95 terms through n = 983 (2026-11-10)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

static inline seq_t A389106_floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (hi * hi <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        if (mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static void A389106_seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[160];
    int pos = 0;
    while (v != 0) {
        tmp[pos++] = (char)('0' + (int)(v % 10));
        v /= 10;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = (size_t)pos;
}

static seq_t A389106_seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v * 10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

#define A389106_CACHE_LIMIT ((size_t)1 << 21)

static seq_t A389106_min_final(seq_t val, seq_t *cache)
{
    if (val < (seq_t)A389106_CACHE_LIMIT) {
        seq_t cached = cache[(size_t)val];
        if (cached != 0)
            return cached;
    }

    char str[160];
    size_t len = 0;
    A389106_seq_to_str(val, str, &len);

    seq_t best = val;

    for (size_t i = 0; i < len; ++i) {
        if (str[i] == '0')
            continue;
        uint64_t sub_val = 0;
        for (size_t j = i; j < len; ++j) {
            if (j - i >= 19)
                break;
            sub_val = sub_val * 10 + (uint64_t)(str[j] - '0');

            seq_t sv = (seq_t)sub_val;
            seq_t r = A389106_floor_sqrt(sv);
            if (r * r == sv) {
                if (sv == r)
                    continue;

                char newbuf[320];
                size_t p = 0;
                memcpy(newbuf, str, i);
                p += i;
                char rootbuf[160];
                size_t rootlen = 0;
                A389106_seq_to_str(r, rootbuf, &rootlen);
                memcpy(newbuf + p, rootbuf, rootlen);
                p += rootlen;
                memcpy(newbuf + p, str + j + 1, len - j - 1);
                p += len - j - 1;
                newbuf[p] = '\0';

                seq_t new_val = A389106_seq_from_str(newbuf);
                seq_t cand = A389106_min_final(new_val, cache);
                if (cand < best)
                    best = cand;

                if (best == (seq_t)1) {
                    if (val < (seq_t)A389106_CACHE_LIMIT)
                        cache[(size_t)val] = (seq_t)1;
                    return (seq_t)1;
                }
            }
        }
    }

    if (val < (seq_t)A389106_CACHE_LIMIT)
        cache[(size_t)val] = best;
    return best;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    if (cache == NULL)
        cache = mem_new(seq_t, A389106_CACHE_LIMIT);

    seq_t limit = (n % 10 == 0) ? (seq_t)100 : (seq_t)10;
    for (idx_t k = 1;; ++k) {
        seq_t v = (seq_t)k * (seq_t)n;
        if (A389106_min_final(v, cache) < limit)
            return (seq_t)k;
    }
}
