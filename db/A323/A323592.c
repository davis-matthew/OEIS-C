/*
 * A323592  n-th digit in the base-4 expansion of 1/n.
 * https://oeis.org/A323592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9761, u16 n <= 9761, u32 n <= 9761, u64 n <= 9761,
 *            u128 n <= 9761, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th digit (1‑indexed) after the decimal point in the
   base‑4 expansion of 1/n.  The algorithm performs long division
   by repeatedly multiplying the current remainder by 4 and taking the
   quotient as the next digit. */
seq_t a(idx_t n)
{
    /* remainders are always < n, so use idx_t for them */
    idx_t r = 1 % n;         /* initial remainder of 1 divided by n */

    seq_t digit = 0;         /* digit produced in the last iteration */

    for (idx_t i = 1; i <= n; ++i) {
        idx_t r4 = r * 4;       /* shift left by 2 bits (multiply by 4) */
        digit = (seq_t)(r4 / n); /* the next base‑4 digit (0–3) */
        r = r4 % n;            /* remainder for the next step */
    }
    return digit;
}
