/*
 * A246083  Paradigm shift sequence for (0,5) production scheme with
 *          replacement.
 * https://oeis.org/A246083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-15) for n >= 25; The first 24 terms are:
 *            1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,22,24,26,28
 * Range:     u8 n <= 54, u16 n <= 129, u32 n <= 281, u64 n <= 583,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n=0, return 0 for safety */

    static const seq_t init[24] = {
        1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
       11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
       22, 24, 26, 28
    };

    if (n <= 24)
        return init[n - 1];

    idx_t k = n;
    seq_t factor = 1;

    while (k > 24) {
        factor *= 3;
        k -= 15;
    }

    return init[k - 1] * factor;
}
