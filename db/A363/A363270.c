/*
 * A363270  The result, starting from n, of Collatz steps x -> (3x+1)/2 while
 *          odd, followed by x -> x/2 while even.
 * https://oeis.org/A363270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = odd part of f⁽ᵗ⁾(n) where * f(x) = (3*x + 1)/2 * t is the
 *            number of trailing 1‑bits of n. * This matches OEIS A363270
 * Range:     u8 n <= 62, u16 n <= 2046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    idx_t t = 0;
    while ((tmp & 1) == 1) {
        tmp >>= 1;
        t++;
    }

    seq_t x = (seq_t)n;
    for (idx_t i = 0; i < t; ++i) {
        x = (3 * x + 1) / 2;
    }

    while ((x & 1) == 0) {
        x >>= 1;
    }
    return x;
}
