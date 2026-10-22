/*
 * A378725  a(n) = A378724(n+1) - A378724(n).
 * https://oeis.org/A378725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = t + 1 , where t is the number of trailing 1’s in the *
 *            base‑3 representation of n, except when n is of the form
 *            (3^k‑1)/2 * (i.e., all digits are 1). In that exceptional case
 *            a(n) = t. * Therefore: * - If n has no trailing 1’s (i.e., n mod
 *            3 is 0 or 2) → a(n) = 1. * - If n has t trailing 1...
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Count trailing 1’s in base‑3. */
    seq_t t = 0;
    while (n % 3 == 1) {
        ++t;
        n /= 3;
    }
    /* If all digits were 1 (n became 0), the answer is t.
       Otherwise add 1 because there is a digit left that is not 1. */
    return (n == 0) ? t : t + 1;
}
