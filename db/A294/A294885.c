/*
 * A294885  a(n) = A004125(n) mod n = [Sum_{i=1..n} (n mod i)] mod n.
 * https://oeis.org/A294885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} (n mod i) mod n
 * Range:     u8 n <= 272, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 108 terms through n = 9086 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        sum += n % i;
    }
    return (seq_t)(sum % n);
}
