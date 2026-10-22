/*
 * A307266  Row 8 of array in A059897.
 * https://oeis.org/A307266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   let e be the exponent of 2 in n (number of trailing zeros). Then
 *            if e % 4 == 0 a(n) = 8 * n if e % 4 == 1 a(n) = 2 * n if e % 4
 *            == 2 a(n) = n / 2 if e % 4 == 3 a(n) = n / 8 This defines the
 *            self‑inverse permutation of the integers that is row 8 of
 *            A059897
 * Range:     u8 n <= 32, u16 n <= 5120, u32 n <= 5120, u64 n <= 5120,
 *            u128 n <= 5120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute exponent of 2 in n */
    idx_t e = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0 && tmp != 0) {
        ++e;
        tmp >>= 1;
    }

    switch (e & 3) {  /* e % 4 */
        case 0: return (seq_t)n * (seq_t)8ULL;
        case 1: return (seq_t)n * (seq_t)2ULL;
        case 2: return (seq_t)n / (seq_t)2ULL;
        default: /* e % 4 == 3 */ return (seq_t)n / (seq_t)8ULL;
    }
}
