/*
 * A308347  n-th digit in the base-6 expansion of 1/n.
 * https://oeis.org/A308347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth digit after the decimal point in the base‑6 expansion
 *            of 1/n (computed by iterated division)
 * Range:     u8 n <= 48, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n=0, return 0 safely */
    seq_t rem   = (seq_t)1 % (seq_t)n;  /* remainder before first digit */
    seq_t digit = 0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t tmp   = rem * (seq_t)6;        /* 6 * current remainder  */
        digit       = tmp / (seq_t)n;        /* next digit            */
        rem         = tmp % (seq_t)n;        /* updated remainder     */
    }
    return digit;
}
