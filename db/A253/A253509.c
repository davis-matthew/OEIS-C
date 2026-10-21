/*
 * A253509  Number of (n+2)X(7+2) 0..1 arrays with every 2X2 and 3X3 subblock
 *          diagonal maximum minus antidiagonal minimum nondecreasing
 *          horizontally and vertically.
 * https://oeis.org/A253509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 11 n^4 + 97 n^3 + 3661 n^2 - 3002 n - 5880) / 6
 *            (valid for all n ≥ 1; the first 12 terms are hard‑coded because
 *            the polynomial only holds for n > 12)
 * Range:     u8 none, u16 n <= 8, u32 n <= 107, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Exact values for n = 1 .. 12, taken directly from the OEIS entry. */
    static const seq_t constant_terms[12] = {
        1500, 3088, 5692, 10236, 16592, 25988,
        38564, 55576, 77680, 106248, 142556, 188308
    };

    if (n <= 12) {
        return constant_terms[n - 1];
    }

    /* Compute n^5 + 11 n^4 + 97 n^3 + 3661 n^2 - 3002 n - 5880
       in the type seq_t and divide by 6. */
    seq_t x   = (seq_t)n;
    seq_t x2  = x * x;          /* n^2  */
    seq_t x3  = x2 * x;         /* n^3  */
    seq_t x4  = x3 * x;         /* n^4  */
    seq_t x5  = x4 * x;         /* n^5  */

    seq_t val = x5
        + (seq_t)11 * x4
        + (seq_t)97 * x3
        + (seq_t)3661 * x2
        - (seq_t)3002 * x
        - 5880;

    return val / 6;
}
