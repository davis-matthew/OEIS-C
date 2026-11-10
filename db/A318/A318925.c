/*
 * A318925  a(n) = A175046(A318921(n)).
 * https://oeis.org/A318925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, hear, base
 * Formula:   a(n) = A175046(A318921(n)), with a(n) = 0 when A318921(n) = 0
 *            (empty string)
 * Range:     u8 n <= 383, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 95 terms through n = 9254 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A318921_helper(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t res = 0;
    int pos = 63;
    while (pos > 0 && !((n >> pos) & 1ULL)) --pos;
    unsigned run_bit = (unsigned)((n >> pos) & 1ULL);
    unsigned run_len = 0;
    for (int i = pos; i >= 0; --i) {
        unsigned bit = (unsigned)((n >> i) & 1ULL);
        if (bit == run_bit) {
            ++run_len;
        } else {
            for (unsigned k = 1; k < run_len; ++k)
                res = (res << 1) | run_bit;
            run_bit = bit;
            run_len = 1;
        }
    }
    for (unsigned k = 1; k < run_len; ++k)
        res = (res << 1) | run_bit;
    return res;
}

static seq_t A175046_helper(uint64_t n)
{
    seq_t result = 0;
    if (n == 0) return result;
    int pos = 63;
    while (pos > 0 && !((n >> pos) & 1ULL)) --pos;
    int i = pos;
    while (1) {
        unsigned bit = (unsigned)((n >> i) & 1ULL);
        unsigned run = 1;
        while (i > 0 && (unsigned)((n >> (i - 1)) & 1ULL) == bit) {
            ++run;
            --i;
        }
        for (unsigned k = 0; k <= run; ++k) {
            result = (result << 1) | (seq_t)bit;
        }
        if (i == 0) break;
        --i;
    }
    return result;
}

seq_t a(idx_t n)
{
    uint64_t m = A318921_helper((uint64_t)n);
    if (m == 0) return (seq_t)0;
    return A175046_helper(m);
}
