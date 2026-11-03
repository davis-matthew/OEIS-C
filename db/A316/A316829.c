/*
 * A316829  Image of 0 under repeated application of the morphism 0 -> 0,1,0,
 *          1 -> 1,1,1.
 * https://oeis.org/A316829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - A088917(n)
 * Range:     u8 n <= 19682, u16 n <= 19682, u32 n <= 19682, u64 n <= 19682,
 *            u128 n <= 19682, big any n
 * Validated: standard, pass, 142 terms through n = 9479 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    while (n > 0) {
        if (n % 3 == 1)
            return (seq_t)1;
        n /= 3;
    }
    return (seq_t)0;
}
