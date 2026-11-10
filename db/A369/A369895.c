/*
 * A369895  Irregular triangle of iteration steps of A063919 until the end of
 *          the terminal cycle is reached, read by rows.
 * https://oeis.org/A369895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   search: row r lists r, A063919(r), A063919(A063919(r)), ... and
 *            stops just before the first value that repeats, so each row ends
 *            with the terminal cycle complete; the rows are concatenated,
 *            with a(1) the first term of row 1
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static uint64_t A063919_step(uint64_t n) {
    if (n == 1) return 1;
    uint64_t temp = n, prod = 1, p = 2;
    while (p <= temp / p) {
        if (temp % p == 0) {
            uint64_t power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= p;
            }
            prod *= 1 + power;
        }
        p += (p == 2 ? 1 : 2);
    }
    if (temp > 1) prod *= 1 + temp;
    return prod - n;
}

static size_t A369895_row(uint64_t r, uint64_t *row, size_t cap) {
    size_t len = 0;
    uint64_t cur = r;
    for (;;) {
        size_t i;
        int seen = 0;
        for (i = 0; i < len; i++) {
            if (row[i] == cur) { seen = 1; break; }
        }
        if (seen || len >= cap) break;
        row[len++] = cur;
        cur = A063919_step(cur);
    }
    return len;
}

seq_t a(idx_t n) {
    static uint64_t row[8192];
    uint64_t idx = (uint64_t)n;
    uint64_t cur_row = 1;
    uint64_t cur_start = 1;
    for (;;) {
        size_t len = A369895_row(cur_row, row, 8192);
        if (idx < cur_start + (uint64_t)len) {
            return (seq_t)row[idx - cur_start];
        }
        cur_start += (uint64_t)len;
        cur_row++;
    }
}
