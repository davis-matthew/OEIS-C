/*
 * A257093  a(n) = n*(n+1)*(13*n+2)/6.
 * https://oeis.org/A257093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(13*n+2)/6
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for all n ≥ 0 with full integer arithmetic.
   The implementation divides early to keep intermediate values
   within the limits of uint64_t and still works for arbitrary‑precision.
   It does not assume any ordering of calls and does not use external
   helpers. */

seq_t a(idx_t n)
{
    /* Convert index to the target integer type. */
    seq_t a = (seq_t)n;
    seq_t b = a + 1;
    seq_t c = (seq_t)13 * a + 2;

    /* Divide by 2, using an even factor. */
    if ((a & 1) == 0)
        a /= 2;
    else
        b /= 2;

    /* Divide by 3, using a factor that is divisible by 3. */
    if (a % 3 == 0)
        a /= 3;
    else if (b % 3 == 0)
        b /= 3;
    else
        c /= 3;   /* 13*n+2 is always divisible by 3 as shown above. */

    /* Final product. */
    return a * b * c;
}
