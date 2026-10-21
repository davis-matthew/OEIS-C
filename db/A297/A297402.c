/*
 * A297402  a(n) = gcd_{k=1..n} (prime(k+1)^n-1)/2.
 * https://oeis.org/A297402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = gcd_{k=1..n} ((prime(k+1)^n - 1)/2)
 * Simplified: a(n) = 1 for odd n; a(n) = 2^{v₂(n)+1} for even n,
 * where v₂(n) is the exponent of 2 in the prime factorisation of n.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, safe fallback */
    if (n & 1) return (seq_t)1;   /* odd n */

    /* compute the exponent of 2 (v₂(n)) */
    idx_t tmp = n;
    unsigned k = 0;
    while ((tmp & 1) == 0) {
        k++;
        tmp >>= 1;
    }

    /* return 2^{k+1} */
    return (seq_t)1 << (k + 1);
}
