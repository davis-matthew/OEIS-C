/*
 * A352262  Integers that need 4 iterations of the map x->A352172(x) to reach
 *          1.
 * https://oeis.org/A352262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer m such that applying x -> A352172(x)
 *            (product of cubes of nonzero digits) first reaches 1 after
 *            exactly 4 iterations; found by incremental search
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A352262_f(seq_t x) {
    seq_t prod = 1;
    while (x > 0) {
        unsigned d = (unsigned)(long long)(x % 10);
        if (d != 0) {
            seq_t c = (seq_t)d * (seq_t)d * (seq_t)d;
            prod *= c;
        }
        x /= 10;
    }
    return prod;
}

static int A352262_ok(idx_t m) {
    seq_t x = (seq_t)m;
    int steps = 0;
    while (x != 1 && steps < 5) {
        x = A352262_f(x);
        steps++;
    }
    return x == 1 && steps == 4;
}

seq_t a(idx_t n) {
    static idx_t last_n = 0;
    static idx_t last_m = 0;
    idx_t cnt, m;
    if (last_n == 0 || n < last_n) {
        cnt = 0;
        m = 0;
    } else {
        cnt = last_n;
        m = last_m;
    }
    while (cnt < n) {
        m++;
        if (A352262_ok(m)) cnt++;
    }
    last_n = cnt;
    last_m = m;
    return (seq_t)m;
}
