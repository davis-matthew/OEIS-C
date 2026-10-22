/*
 * A323591  n-th digit after the radix point in the base-3 expansion of 1/n.
 * https://oeis.org/A323591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 8889, u32 n <= 8889, u64 n <= 8889,
 *            u128 n <= 8889, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the n-th digit after the radix point in the base‑3
   expansion of 1/n by simulating base‑3 long division. */
seq_t a(idx_t n)
{
    if (n == 0u) return 0;          /* undefined by sequence offset, but safe */
    seq_t rem = (seq_t)1 % n;       /* remainder after integer part (0 if n divides 1) */
    seq_t digit = 0;
    for (idx_t i = 0; i < n; ++i) { /* n steps → n‑th digit */
        rem *= 3;                   /* shift remainder left by one base‑3 place */
        digit = rem / n;            /* digit is the integer part of the division */
        rem   %= n;                 /* new remainder for next step */
    }
    return digit;
}
