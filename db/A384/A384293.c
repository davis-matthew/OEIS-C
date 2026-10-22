/*
 * A384293  Consecutive internal states of the pseudo-random number generator
 *          (214013*(s mod 2^16)+13523655) mod 2^24 when started at s=1.
 * https://oeis.org/A384293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; * for n > 1, a(n) = (214013 * (a(n-1) mod 2^16) +
 *            13523655) mod 2^24
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 9813, u64 n <= 9813,
 *            u128 n <= 9813, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t MASK16 = (seq_t)0xFFFF;
    static const seq_t MASK24 = (seq_t)0xFFFFFF;
    static const seq_t MUL    = (seq_t)214013;
    static const seq_t ADD    = (seq_t)13523655;

    seq_t s = 1;
    for (idx_t i = 2; i <= n; ++i) {
        s = (MUL * (s & MASK16) + ADD) & MASK24;
    }
    return s;
}
