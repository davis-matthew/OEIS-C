/*
 * A339013  Class number m containing n in a partitioning of the natural
 *          numbers into classes B_m by William J. Keith.
 * https://oeis.org/A339013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 2 + the number of trailing non‑zero digits in the
 *            factorial‑base representation of (n-1)
 * Range:     u8 n <= 9871, u16 n <= 9871, u32 n <= 9871, u64 n <= 9871,
 *            u128 n <= 9871, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based. Guard against zero, though OEIS starts at 1. */
    if (n == 0) return (seq_t)0;

    idx_t x = n - 1;
    int t = 0;                     /* count of consecutive non‑zero digits */

    for (idx_t k = 1; x > 0; ++k) {
        idx_t digit = x % (k + 1); /* factorial base digit a_k */
        if (digit == 0) break;     /* trailing zero found */
        ++t;                       /* another non‑zero digit */
        x /= (k + 1);
    }

    return (seq_t)(t + 2); /* convert to seq_t */
}
