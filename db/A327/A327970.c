/*
 * A327970  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A003415(n) = A003415(j) and A327936(i) = A327936(j) for all i,
 *          j.
 * https://oeis.org/A327970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pairs
 *            [A003415(n), A327936(n)]; computed by scanning m=1..n and
 *            numbering distinct pairs by first occurrence
 * Range:     u8 n <= 95, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8527 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A003415_a(idx_t n) {
    if (n == 0 || n == 1)
        return 0;
    seq_t result = 0;
    seq_t original_n = (seq_t)n;
    seq_t remaining = original_n;
    for (seq_t p = 2; p * p <= remaining; p++) {
        int exponent = 0;
        while (remaining % p == 0) {
            exponent++;
            remaining /= p;
        }
        if (exponent > 0) {
            result += (original_n / p) * (seq_t)exponent;
        }
    }
    if (remaining > 1)
        result += original_n / remaining;
    return result;
}

static seq_t A327936_a(idx_t n) {
    if (n < 1)
        return 0;
    seq_t prod = 1;
    uint64_t x = n;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            uint64_t ex = 0;
            while (x % p == 0) {
                x /= p;
                ++ex;
            }
            if (ex >= p)
                prod *= (seq_t)p;
        }
    }
    return prod;
}

seq_t a(idx_t n) {
    if (n < 1)
        return 0;
    seq_t *keys = mem_new(seq_t, (size_t)(2 * n));
    size_t cnt = 0;
    seq_t result = 0;
    for (idx_t m = 1; m <= n; m++) {
        seq_t d = A003415_a(m);
        seq_t e = A327936_a(m);
        size_t found = cnt;
        for (size_t i = 0; i < cnt; i++) {
            if (keys[2 * i] == d && keys[2 * i + 1] == e) {
                found = i;
                break;
            }
        }
        if (found == cnt) {
            keys[2 * cnt] = d;
            keys[2 * cnt + 1] = e;
            cnt++;
        }
        if (m == n)
            result = (seq_t)(found + 1);
    }
    mem_free(keys);
    return result;
}
