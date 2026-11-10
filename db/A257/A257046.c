/*
 * A257046  Numbers having trace 1 in their enhanced squares representation,
 *          see A256913.
 * https://oeis.org/A257046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 0 whose decreasing enhanced squares
 *            representation (A256913) ends in 1 (trace 1, A256914(k)=1);
 *            found by direct search
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9430 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A257046_isqrt(idx_t x)
{
    idx_t r = 0;
    idx_t bit = ((idx_t)1) << 62;
    while (bit != 0) {
        idx_t next = r + bit;
        if (next <= x / next) {
            r = next;
        }
        bit >>= 1;
    }
    return r;
}

static idx_t A257046_find_base(idx_t rem, idx_t prev)
{
    idx_t s = A257046_isqrt(rem);
    idx_t square = s * s;
    while (square >= prev && s > 0) {
        --s;
        square = s * s;
    }
    idx_t base = square;
    if (rem >= 3 && (idx_t)3 < prev && (idx_t)3 > base) base = 3;
    if (rem >= 2 && (idx_t)2 < prev && (idx_t)2 > base) base = 2;
    if (rem >= 1 && (idx_t)1 < prev && (idx_t)1 > base) base = 1;
    return base;
}

static int A257046_ends_in_one(idx_t k)
{
    idx_t rem = k;
    idx_t prev = k + 1;
    idx_t last = 0;
    while (rem > 0) {
        idx_t e = A257046_find_base(rem, prev);
        last = e;
        rem -= e;
        prev = e;
    }
    return last == 1;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        if (A257046_ends_in_one(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}
