/*
 * A258547  Number of (n+1)X(1+1) 0..1 arrays with every 2X2 subblock ne-sw
 *          antidiagonal difference nondecreasing horizontally and nw+se
 *          diagonal sum nondecreasing vertically.
 * https://oeis.org/A258547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*2^n - 4*n - 12 (valid for all n >= 1)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generic exponentiation by squaring: computes 2^exp in seq_t */
static seq_t pow2_generic(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 2;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1; n==0 is undefined but harmless */
    seq_t val = pow2_generic(n);    /* 2^n */
    val <<= 4;                      /* multiply by 16 (2^4) */
    val -= (seq_t)4 * (seq_t)n;     /* subtract 4*n */
    val -= 12;                      /* subtract 12 */
    return val;
}
