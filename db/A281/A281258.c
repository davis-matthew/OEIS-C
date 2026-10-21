/*
 * A281258  Digital root of n*(n+1)*(n+2)/2.
 * https://oeis.org/A281258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital root of n*(n+1)*(n+2)/2
 * Range:     u8 n <= 117, u16 n <= 117, u32 n <= 117, u64 n <= 117,
 *            u128 n <= 117, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the only case where the digital root is zero */
    if (n == 0)
        return 0;

    /* compute (n*(n+1)*(n+2)/2) mod 9 efficiently
     * using the fact that 2 has inverse 5 modulo 9 (since 2*5 ≡ 1 (mod 9)) */
    idx_t r0 = n % 9;
    idx_t r1 = (n + 1) % 9;
    idx_t r2 = (n + 2) % 9;

    idx_t prod = (r0 * r1) % 9;
    prod = (prod * r2) % 9;
    prod = (prod * 5) % 9;   /* division by 2 */

    /* map the remainder 0 to the digital root 9 for non‑zero arguments */
    return (prod == 0) ? (seq_t)9 : (seq_t)prod;
}
