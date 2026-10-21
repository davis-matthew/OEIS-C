/*
 * A273149  a(n) = A053839(n)+1.
 * https://oeis.org/A273149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum of base‑4 digits of n) mod 4 + 1
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t x = n;
    while (x) {
        sum += x & (idx_t)3;   /* get lowest base‑4 digit */
        x >>= 2;              /* shift to next two bits (next digit) */
    }
    return (sum % (seq_t)4) + (seq_t)1;
}
