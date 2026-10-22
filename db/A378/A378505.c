/*
 * A378505  a(0) = 0, a(n) = 0 where a(n-1) is nonzero and divisible by n.
 *          Otherwise a(n) = n + a(n-1).
 * https://oeis.org/A378505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(0) = 0; for n > 0 * if a(n‑1) ≠ 0 and a(n‑1) % n == 0 then
 *            a(n) = 0 * otherwise a(n) = a(n‑1) + n * This is a direct
 *            iterative implementation that works for any seq_t, * whether it
 *            is a fixed‑size unsigned integer or an arbitrary‑precision * big
 *            integer class
 * Range:     u8 n <= 25, u16 n <= 508, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Compute OEIS A378505 */
seq_t a(idx_t n)
{
    seq_t value = (seq_t)0;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* cast i to seq_t for the modulus and addition */
        seq_t step = (seq_t)i;
        if (value != (seq_t)0 && (value % step) == (seq_t)0) {
            value = (seq_t)0;
        } else {
            value += step;
        }
    }
    return value;
}
