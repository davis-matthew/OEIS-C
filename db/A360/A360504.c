/*
 * A360504  Concatenate the ternary strings for 1,2,...,n-1, n, n-1, ..., 2,1.
 * https://oeis.org/A360504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the ternary representations of
 *            1,2,…,n-1,n,n-1,…,2,1, with the resulting decimal value
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 122 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Append the digits of x written in base‑3 to the decimal number *res.
   The most significant digit is processed first. */
static inline void append_ternary_digits(seq_t *res, idx_t x)
{
    /* Since x fits in idx_t, at most 64 ternary digits are needed. */
    unsigned int digs[64];
    int d = 0;

    /* x ≥ 1 in all calls, but handle x == 0 safely. */
    if (x == 0) {
        *res = (*res) * 10;
        return;
    }

    while (x) {
        digs[d++] = x % 3;
        x      /= 3;
    }

    /* Process digits from most significant to least. */
    for (int i = d - 1; i >= 0; --i) {
        *res = (*res) * 10 + digs[i];
    }
}

seq_t a(idx_t n)
{
    seq_t res = 0;

    /* Forward part: 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        append_ternary_digits(&res, i);
    }

    /* Reverse part: n-1 .. 1 */
    for (idx_t i = n - 1; i > 0; --i) {
        append_ternary_digits(&res, i);
    }

    return res;
}
