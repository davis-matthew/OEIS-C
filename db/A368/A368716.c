/*
 * A368716  a(n) = n! * Sum_{k=0..n} (-1)^(n-k) * k^3 / k!.
 * https://oeis.org/A368716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368716
   a(0) = 0
   a(n) = -n * a(n-1) + n^3   (n ≥ 1)
   (Recurrence from the OEIS entry) */

#include <stddef.h> /* for size_t */

/* External types supplied by the build system */
extern seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t prev = (seq_t)0;         /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t seq_i = (seq_t)i;
        seq_t i3 = seq_i * seq_i * seq_i; /* n^3, safe for n ≤ 2^21 on 64‑bit */
        seq_t cur = -seq_i * prev + i3;   /* recurrence */
        prev = cur;
    }
    return prev;
}
