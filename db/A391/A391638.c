/*
 * A391638  Binary encoding of the disks missing from pin 1, after n moves,
 *          when moving infinitely many disks in the Tower of Hanoi starting
 *          from pin 1, via the optimal algorithm.
 * https://oeis.org/A391638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Gray(n) XOR Gray(n/3) where Gray(k) = k ^ (k >> 1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t g1 = x ^ (x >> 1);

    idx_t b = n / 3;
    seq_t y = (seq_t)b;
    seq_t g2 = y ^ (y >> 1);

    return g1 ^ g2;
}
