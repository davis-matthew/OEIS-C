/*
 * A375265  a(n) = n/3 if n mod 3 = 0; otherwise a(n) = n/2 if n mod 2 = 0;
 *          otherwise a(n) = 3*n + 1.
 * https://oeis.org/A375265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/3 if n mod 3 = 0 * a(n) = n/2 if n mod 3 ≠ 0 and n mod
 *            2 = 0 * a(n) = 3*n+1 if n mod 3 ≠ 0 and n mod 2 ≠ 0 * * Offset:
 *            1 (a(1) is the first listed term)
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n is guaranteed to be ≥ 1.  We use modulo on the 64‑bit index
     * because that operation is well defined for all n that may
     * appear in the test harness.  The result is then converted
     * to seq_t, which may be 8, 16, 32, 64 or 128 bits or an
     * arbitrary‑precision integer, guaranteeing correctness as long
     * as the true value fits into seq_t. */
    if (n % 3 == 0)
        return (seq_t)(n / 3);
    if (n % 2 == 0)
        return (seq_t)(n / 2);

    /* For odd n that are not multiples of 3.  Use seq_t arithmetic
     * so that the multiplication and addition are performed in the
     * target integer type.  This keeps intermediate results from
     * exceeding the width of seq_t when the final result is known
     * to fit. */
    seq_t m = (seq_t)n;
    return (seq_t)(3 * m + 1);
}
