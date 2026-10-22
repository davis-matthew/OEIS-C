/*
 * A358730  Positions of first appearances in A358729 (number of nodes minus
 *          node-height).
 * https://oeis.org/A358730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=1; a(2)=4; a(3)=8; a(4)=16; For n>=5: if n is even: a(n) =
 *            2 * 3^(n/2) if n is odd: a(n) = 3^((n+1)/2)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow3(idx_t exp) {
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;
    idx_t e = exp;
    while (e) {
        if (e & 1U) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)16;

    if ((n & 1U) == 0) {          /* even n >= 6 */
        idx_t k = n >> 1;
        seq_t p = pow3(k);
        return p * (seq_t)2;
    } else {                       /* odd n >= 5 */
        idx_t k = (n + 1) >> 1;
        return pow3(k);
    }
}
