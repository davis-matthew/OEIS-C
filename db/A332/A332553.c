/*
 * A332553  a(n) = n + A082183(n) - A082184(n).
 * https://oeis.org/A332553
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + A082183(n) - A082184(n)
 * Range:     u8 n <= 351, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the smallest positive integer a < n such that
   a*(1 + 2n - a) is divisible by 2*(n - a), i.e. the
   expression m = a*(1 + 2n - a) / (2*(n - a)) is an integer. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* offset starts at 2 */
    for (idx_t a = 1; a < n; ++a)
    {
        idx_t num = a * (((idx_t)1 + ((idx_t)2 * n)) - a);
        idx_t den = (idx_t)2 * (n - a);
        if (num % den == 0)
            return (seq_t)a;
    }
    /* fallback: a = n-1 always works */
    return (seq_t)(n - 1);
}
