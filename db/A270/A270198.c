/*
 * A270198  a(n) = A054429(A055938(A054429(n))).
 * https://oeis.org/A270198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A054429(A055938(A054429(n))); A055938 found by a linear
 *            merge scan against b(k) = 2k - popcount(k) (complement of
 *            A005187)
 * Range:     u8 n <= 127, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 90 terms through n = 8100 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static seq_t rev_block(seq_t v)
{
    seq_t start = 1;
    if (v == 0) return 0;
    while (start * 2 <= v) start <<= 1;
    return start * 3 - 1 - v;
}

static unsigned pc_seq(seq_t k)
{
    unsigned c = 0;
    while (k != 0) {
        if ((k & 1) != 0) c++;
        k >>= 1;
    }
    return c;
}

/* m-th positive integer not of the form 2k - popcount(k) */
static seq_t nth_missing(seq_t m)
{
    seq_t k = 0, fk = 0, x = 1, missing = 0;
    for (;;) {
        while (fk < x) {
            k++;
            fk = k * 2 - (seq_t)pc_seq(k);
        }
        if (fk != x) {
            missing++;
            if (missing == m) return x;
        }
        x++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t m = rev_block((seq_t)n);
    seq_t r = nth_missing(m);
    return rev_block(r);
}
