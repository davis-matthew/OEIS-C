/*
 * A353826  The positions of nonzero digits in the ternary expansions of n and
 *          a(n) are the same, and the k-th rightmost nonzero digit in a(n)
 *          equals modulo 3 the product of the k rightmost nonzero digits in
 *          n.
 * https://oeis.org/A353826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 244, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum over nonzero ternary digits d of n of
 *                 (product of all nonzero digits of n up to that digit modulo 3)
 *                 * 3^position.
 * Recurrence: a(3*n) = 3*a(n), a(3*n+1) = 3*a(n)+1.
 * This implementation follows the definition directly.
 */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t result = 0;
    seq_t prod = 1;          /* current product modulo 3 */
    seq_t pow3 = 1;          /* 3^position */
    while (n) {
        idx_t digit = n % 3;
        n /= 3;
        if (digit != 0) {
            prod = (prod * (seq_t)digit) % (seq_t)3;
            result += prod * pow3;
        }
        pow3 *= 3;
    }
    return result;
}
