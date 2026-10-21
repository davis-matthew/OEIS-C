/*
 * A257181  Expansion of (1 - x) * (1 + x^4) / (1 + x^5) in powers of x.
 * https://oeis.org/A257181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1 For n > 0 let k = n / 5, r = n % 5. Let s = (-1)^k,
 *            i.e. s = 1 if k even, -1 otherwise. Then: r == 0 : a(n) = 2 * s
 *            (n = 5*k, k>0) r == 1 : a(n) = -s (n = 5*k+1) r == 2 : a(n) = 0
 *            r == 3 : a(n) = 0 r == 4 : a(n) = s (n = 5*k+4)
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    idx_t k = n / 5;
    int s = ((k & 1ULL) == 0) ? 1 : -1;  /* (-1)^k */
    int r = n % 5;
    switch (r) {
        case 0: return (seq_t)(2 * s);
        case 1: return (seq_t)(-s);
        case 2: return (seq_t)0;
        case 3: return (seq_t)0;
        case 4: return (seq_t)(s);
        default: return (seq_t)0; /* unreachable */
    }
}
