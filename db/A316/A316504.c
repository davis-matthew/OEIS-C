/*
 * A316504  Numbers k such that A090086(k+1) > k+1.
 * https://oeis.org/A316504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k >= 0 such that A090086(k+1) > k+1, i.e. no
 *            composite x <= k+1 has (k+1)^(x-1) == 1 (mod x); found by
 *            search, k composite (k>2) skipped
 * Range:     u8 n <= 27, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A316504_pm(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = (uint64_t)(((unsigned __int128)r * b) % m);
        b = (uint64_t)(((unsigned __int128)b * b) % m);
        e >>= 1;
    }
    return r;
}

static bool A316504_composite(uint64_t x) {
    if (x < 4) return false;
    if (x % 2 == 0) return true;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return true;
    return false;
}

static bool A316504_prime(uint64_t x) {
    return x >= 2 && !A316504_composite(x);
}

static bool A316504_good(uint64_t k) {
    uint64_t m = k + 1;
    if (k > 2 && !A316504_prime(k)) return false;
    for (uint64_t x = 4; x <= m; x++) {
        if (A316504_pm(m, x - 1, x) == 1 && A316504_composite(x))
            return false;
    }
    return true;
}

static uint64_t *A316504_cache = NULL;
static uint64_t A316504_cnt = 0, A316504_cap = 0, A316504_next = 0;

seq_t a(idx_t n) {
    while (A316504_cnt < (uint64_t)n) {
        uint64_t k = A316504_next++;
        if (A316504_good(k)) {
            if (A316504_cnt == A316504_cap) {
                A316504_cap = A316504_cap ? A316504_cap * 2 : 64;
                A316504_cache = (uint64_t *)realloc(A316504_cache, A316504_cap * sizeof(uint64_t));
            }
            A316504_cache[A316504_cnt++] = k;
        }
    }
    return (seq_t)A316504_cache[n - 1];
}
