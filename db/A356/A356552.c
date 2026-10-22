/*
 * A356552  a(n) is the least base b > 1 where the sum of digits of n divides
 *          n.
 * https://oeis.org/A356552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least base b > 1 such that the sum of the base‑b digits
   of n divides n.  Brute‑force search over b. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;          /* a(1) = 2 by definition */

    seq_t sn = (seq_t)n;          /* n as seq_t for modulo operation */

    for (idx_t b = 2; b <= n; ++b)
    {
        idx_t  tmp = n;
        seq_t sum = 0;

        /* compute digit sum of n in base b */
        while (tmp)
        {
            sum += (seq_t)(tmp % b);
            tmp /= b;
        }

        /* check divisibility */
        if (sn % sum == 0)
            return (seq_t)b;      /* found the minimal base */
    }

    /* Fallback – occurs only for n = 1 (handled above) or unexpected case */
    return sn;
}
