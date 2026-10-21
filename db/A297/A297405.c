/*
 * A297405  Binary "cubes"; numbers whose binary representation consists of
 *          three consecutive identical blocks.
 * https://oeis.org/A297405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * (1 + 2^p + 4^p) where p = 1 + floor(log₂(n))
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* compute floor(log₂(n)) as the number of shifts needed to reduce n to 1 */
    unsigned log2 = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++log2;
    }

    /* p = 1 + floor(log₂(n)) */
    unsigned p = log2 + 1;

    /* compute 2^p and 4^p in seq_t using shifts.
       For normal 64‑bit seq_t we guard against shifting more than 63 bits
       (which would be UB).  For larger seq_t types (e.g. arbitrary‑precision)
       the shift is safe for any count. */
    seq_t one  = (seq_t)1;
    seq_t two_pow, four_pow;

    if (sizeof(seq_t) == 8) {            /* 64‑bit unsigned type */
        if (p < 64 && (2U * p) < 64) {
            two_pow  = one << p;
            four_pow = one << (2U * p);
        } else {
            /* fall back to iterative shifting to avoid UB on large shifts */
            two_pow  = one;
            for (unsigned i = 0; i < p; ++i)
                two_pow <<= 1;
            four_pow = one;
            for (unsigned i = 0; i < 2U * p; ++i)
                four_pow <<= 1;
        }
    } else {
        two_pow  = one << p;
        four_pow = one << (2U * p);
    }

    seq_t sum = one + two_pow + four_pow;
    return (seq_t)n * sum;
}
