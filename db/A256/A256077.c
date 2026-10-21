/*
 * A256077  Repeat 2^d times the repunit A002275(d); d = 1, 2, 3...
 * https://oeis.org/A256077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = repunit with d digits, where d is the smallest integer
 *            such that 2^(d+1) – 2 ≥ n. The block of d has size 2^d
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 2046, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* locate the block containing n (blocks start with 1, 2, 3, ...) */
    idx_t cum = 0;
    idx_t d = 1;
    while (cum + (1ULL << d) < n) {
        cum += (1ULL << d);
        d++;
    }

    /* build the d‑digit repunit 111...1 */
    seq_t rep = 0;
    for (idx_t i = 0; i < d; ++i) {
        rep = rep * (seq_t)10 + (seq_t)1;
    }
    return rep;
}
