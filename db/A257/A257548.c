/*
 * A257548  a(1) = 1, a(2) = 2, a(3) = 5, a(4) = 8 and a(5) = 15, a(n) =
 *          Sum_{j=1..n-1} a(j).
 * https://oeis.org/A257548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=5, a(4)=8, a(5)=15; a(n)=31*2^(n-6) for
 *            n>=6
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Exponentiation by squaring for 2^exp, safe for any seq_t type */
static inline seq_t pow2_idx(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 2;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    switch (n) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        case 3: return (seq_t)5;
        case 4: return (seq_t)8;
        case 5: return (seq_t)15;
        default: /* n >= 6 */
            return (seq_t)31 * pow2_idx(n - 6);
    }
}
