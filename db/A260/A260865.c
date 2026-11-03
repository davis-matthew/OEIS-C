/*
 * A260865  Base-15 representation of a(n) is the concatenation of the base-15
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of base‑15 representations of
 *            1,2,…,n,n‑1,…,1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the value by repeatedly appending digits in base 15. */

#include <stdint.h>

/* Return the number of base‑15 digits of a positive number x. */
static inline int digits15(idx_t x)
{
    int d = 0;
    do {
        ++d;
        x /= 15;
    } while (x);
    return d;
}

/* Compute 15^d where d is a small non‑negative integer. */
static inline seq_t pow15_int(int d)
{
    seq_t p = 1;
    for (int i = 0; i < d; ++i)
        p *= 15;
    return p;
}

/* OEIS sequence A260865. */
seq_t a(idx_t n)
{
    seq_t res = 0;

    /* Append numbers 1 .. n. */
    for (idx_t i = 1; i <= n; ++i) {
        int d = digits15(i);
        seq_t mult = pow15_int(d);
        res = res * mult + (seq_t)i;
    }

    /* Append numbers n‑1 .. 1. */
    if (n > 0) {
        for (idx_t i = n - 1; i > 0; --i) {
            int d = digits15(i);
            seq_t mult = pow15_int(d);
            res = res * mult + (seq_t)i;
        }
    }

    return res;
}
