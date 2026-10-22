/*
 * A391536  Integers x such that there exist four integers 0<y<=z<=t<=w such
 *          that psi(x)^4 = x^4 + y^4 + z^4 + t^4 + w^4.
 * https://oeis.org/A391536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A391536:  a(1) = 538;
    For n ≥ 2, a(n) = 538 × 2^(n+4).  */

/*  The numbers x = 538·2^k (k ≥ 0) generate psi‑quartic quintuples by
    scaling the base quadruple (538, 96, 532, 548, 648).  */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for n < 1; return 0 for safety */
        return 0;
    if (n == 1)
        return (seq_t)538;          /* a(1) = 538 */

    seq_t val = (seq_t)538;         /* start with the base value */
    /*  Multiply by 2^(n+4).  The loop counts total of (n+4) doublings. */
    for (idx_t i = 0; i < n + 4; ++i)
        val = val * 2;

    return val;
}
