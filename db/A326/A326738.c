/*
 * A326738  Numbers which converge to 5 under repeated application of the
 *          powertrain map of A133500.
 * https://oeis.org/A326738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 24, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A326738: Numbers that converge to 5 under repeated application of the
 * powertrain map of A133500.
 * The powertrain map is defined for a decimal number d_k…d_1 as:
 *   f(n) = (d_1^d_2)*(d_3^d_4)*…*(d_{2i-1}^d_{2i})   * (d_{last})   if the number of digits is odd.
 * If the number of digits is even the final digit is omitted.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

static seq_t next_powertrain(seq_t x)
{
    char buf[64];
    size_t len;
    seq_to_str(x, buf, &len);

    seq_t prod = 1;
    for (size_t i = 0; i + 1 < len; i += 2) {
        unsigned int base = (unsigned int)(buf[i] - '0');
        unsigned int exp  = (unsigned int)(buf[i + 1] - '0');
        prod *= ipow_u64_idx(base, exp);
    }
    if (len % 2 == 1) {
        unsigned int last = (unsigned int)(buf[len - 1] - '0');
        prod *= (seq_t)last;
    }
    return prod;
}

static bool converges_to_5(seq_t x)
{
    const seq_t TARGET = (seq_t)5;
    for (int step = 0; step < 20; ++step) {
        if (x == TARGET) return true;
        x = next_powertrain(x);
    }
    return x == (seq_t)5;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but safe guard */

    idx_t found = 0;
    for (uint64_t cand = 5; ; ++cand) {
        if (converges_to_5((seq_t)cand)) {
            ++found;
            if (found == n) return (seq_t)cand;
        }
    }
}
