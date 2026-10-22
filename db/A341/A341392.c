/*
 * A341392  a(n) = A284005(n) / (1 + A000120(n))!.
 * https://oeis.org/A341392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A284005(n)/(1 + popcount(n))! Recurrence: a(0)=1, a(2n) =
 *            (1 + popcount(n)) * a(n), a(2n+1) = a(n)
 * Range:     u8 n <= 111, u16 n <= 1791, u32 n <= 9746, u64 n <= 9746,
 *            u128 n <= 9746, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any non‑negative index n. */
#include <stdint.h>

static inline unsigned popcount_u64(idx_t x) {
    unsigned c = 0;
    while (x) {
        c++;
        x &= x - 1;
    }
    return c;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    while (n != 0) {
        if (n & 1) {          /* odd: a(n) = a(n>>1) */
            n >>= 1;
        } else {              /* even: a(n) = (1+popcount(n>>1)) * a(n>>1) */
            idx_t m = n >> 1;
            unsigned pc = popcount_u64(m);
            res *= (seq_t)(1 + pc);
            n = m;
        }
    }
    return res;
}
