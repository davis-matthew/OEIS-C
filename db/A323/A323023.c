/*
 * A323023  Irregular triangle read by rows where row n is the omega-sequence
 *          of n.
 * https://oeis.org/A323023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   flattened rows; row m lists Omega(m), Omega(red(m)), ... (red =
 *            A181819, Omega = A001222) until a prime (Omega = 1) is reached;
 *            row 1 is empty
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static uint64_t A323023_nth_prime(uint64_t k) {
    uint64_t c = 1, cnt = 0;
    while (cnt < k) {
        c++;
        int ok = 1;
        for (uint64_t d = 2; d * d <= c; d++) {
            if (c % d == 0) { ok = 0; break; }
        }
        if (ok) cnt++;
    }
    return c;
}

/* returns Omega(m), sets *red to prime shadow of m */
static uint64_t A323023_step(uint64_t m, uint64_t *red) {
    uint64_t om = 0, r = 1;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) { t /= p; e++; }
            om += e;
            r *= A323023_nth_prime(e);
        }
    }
    if (t > 1) { om += 1; r *= 2; }
    *red = r;
    return om;
}

static size_t A323023_row(uint64_t m, uint64_t *out) {
    size_t len = 0;
    while (m > 1) {
        uint64_t r;
        uint64_t om = A323023_step(m, &r);
        out[len++] = om;
        if (om == 1) break;
        m = r;
    }
    return len;
}

seq_t a(idx_t n) {
    static uint64_t cache_row = 2;
    static uint64_t cache_start = 1; /* index of first term of cache_row */
    uint64_t row, start;
    if ((uint64_t)n >= cache_start) { row = cache_row; start = cache_start; }
    else { row = 2; start = 1; }
    uint64_t buf[128];
    for (;;) {
        size_t len = A323023_row(row, buf);
        if ((uint64_t)n < start + len) {
            cache_row = row;
            cache_start = start;
            return (seq_t)buf[(size_t)((uint64_t)n - start)];
        }
        start += len;
        row++;
    }
}
