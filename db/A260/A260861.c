/*
 * A260861  Base-11 representation of a(n) is the concatenation of the base-11
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = integer whose base‑11 representation is the concatenation
 *            of the base‑11 representations of 1,2,…,n,n-1,…,1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: number of digits of x in base 11 (x>0) */
static int digits_in_base11(idx_t x) {
    int d = 1;
    while (x >= 11) {
        x /= 11;
        ++d;
    }
    return d;
}

/* Helper: 11^len in seq_t */
static seq_t pow11_of_len(int len) {
    seq_t r = 1;
    while (len-- > 0)
        r = r * 11;
    return r;
}

seq_t a(idx_t n) {
    seq_t res = 0;

    /* ascending part: 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        int d = digits_in_base11(i);
        seq_t mult = pow11_of_len(d);
        res = res * mult + (seq_t)i;
    }

    /* descending part: n-1 .. 1 */
    for (idx_t i = n; i > 1; --i) {
        int d = digits_in_base11(i - 1);
        seq_t mult = pow11_of_len(d);
        res = res * mult + (seq_t)(i - 1);
    }

    return res;
}
