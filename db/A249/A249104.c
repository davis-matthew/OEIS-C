/*
 * A249104  Defective numbers: A complete residue system mod a(n) does not
 *          exist in the Fibonacci sequence.
 * https://oeis.org/A249104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 1 with A066853(m) < m; found by search,
 *            counting distinct Fibonacci residues mod m over one Pisano
 *            period
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t A249104_resid_count(uint64_t m)
{
    bool *seen = (bool *)malloc((size_t)m * sizeof(bool));
    if (!seen) return m;
    for (size_t i = 0; i < (size_t)m; ++i) seen[i] = false;
    uint64_t x = 0, y = 1 % m, count = 1;
    seen[0] = true;
    while (true) {
        uint64_t nx = (x + y) % m;
        x = y;
        y = nx;
        if (!seen[(size_t)x]) {
            seen[(size_t)x] = true;
            ++count;
        }
        if (x == 0 && y == 1 % m) break;
    }
    free(seen);
    return count;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0;
    uint64_t m = 0;
    while (cnt < (uint64_t)n) {
        ++m;
        if (A249104_resid_count(m) < m) ++cnt;
    }
    return (seq_t)m;
}
