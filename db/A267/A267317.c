/*
 * A267317  a(n) = final digit of 2^n-1.
 * https://oeis.org/A267317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (2^n - 1) mod 10; a(0)=0, and for n>0 a(n) repeats
 *            1,3,7,5 every 4 indices
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }
    switch (n % 4ULL) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)3;
        case 3: return (seq_t)7;
        default: /* n % 4 == 0 */
            return (seq_t)5;
    }
}
