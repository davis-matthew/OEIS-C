/*
 * A301708  Coordination sequence for node of type V1 in "krc" 2-D tiling (or
 *          net).
 * https://oeis.org/A301708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0: if n is even (n = 2*t) -> a(n) = 11*t if n
 *            mod 4 == 1 (n = 4*t+1) -> a(n) = 22*t + 6 if n mod 4 == 3 (n =
 *            4*t+3) -> a(n) = 22*t + 16
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* even n : n = 2*t */
    if ((n & 1) == 0) {
        idx_t t = n >> 1;                      /* t = n/2 */
        return (seq_t)11 * (seq_t)t;
    }

    /* odd n */
    if ((n & 3) == 1) {                         /* n = 4*t + 1 */
        idx_t t = (n - 1) >> 2;                /* t = (n-1)/4 */
        return (seq_t)22 * (seq_t)t + (seq_t)6;
    }

    /* remaining case: n mod 4 == 3 */
    idx_t t = (n - 3) >> 2;                    /* t = (n-3)/4 */
    return (seq_t)22 * (seq_t)t + (seq_t)16;
}
