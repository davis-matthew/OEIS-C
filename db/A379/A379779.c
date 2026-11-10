/*
 * A379779  Numbers k >= 1 such that A007088(k) / A007088(A007953(k)) is an
 *          integer.
 * https://oeis.org/A379779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k >= 1 such that A007088(k) / A007088(A007953(k)) is
 *            an integer; computed by a forward search over k, resuming from
 *            the last result
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A007088_a(seq_t n) {
    seq_t result = 0;
    seq_t power_of_10 = 1;
    while (n > 0) {
        if (n & 1) {
            result += power_of_10;
        }
        n >>= 1;
        power_of_10 *= 10;
    }
    return result;
}

static seq_t A007953_a(seq_t num) {
    seq_t sum = 0;
    while (num > 0) {
        sum += num % 10;
        num /= 10;
    }
    return sum;
}

static seq_t cache_k = 0;
static idx_t cache_n = 0;

seq_t a(idx_t n) {
    seq_t k;
    idx_t cnt;
    if (cache_n != 0 && cache_n <= n) {
        k = cache_k;
        cnt = cache_n;
    } else {
        k = 0;
        cnt = 0;
    }
    while (cnt < n) {
        k++;
        seq_t ds = A007953_a(k);
        seq_t d = A007088_a(ds);
        if (d == 0) continue;
        seq_t b = A007088_a(k);
        if (b % d == 0) {
            cnt++;
        }
    }
    cache_k = k;
    cache_n = cnt;
    return k;
}
