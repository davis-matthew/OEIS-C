/*
 * A247004  Denominator of (n+4)/gcd(n, 4)^2, a 16-periodic sequence that
 *          associates A061037 with A106617.
 * https://oeis.org/A247004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = denominator of (n+4)/gcd(n,4)^2 The sequence is
 *            16‑periodic: a(n)=1 if n is odd a(n)=2 if n≡2 (mod 4)
 *            a(n)=4,2,4,1 for n≡0,4,8,12 (mod 16) respectively
 * Range:     u8 n <= 9689, u16 n <= 9689, u32 n <= 9689, u64 n <= 9689,
 *            u128 n <= 9689, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fast parity tests */
    if ((n & 1ULL) != 0ULL)               /* odd */
        return (seq_t)1;
    if ((n & 3ULL) == 2ULL)              /* n≡2 mod 4 */
        return (seq_t)2;

    /* n≡0 mod 4 */
    switch (n & 15ULL)                    /* n mod 16 */
    {
        case 0:  return (seq_t)4;          /* n≡0 mod 16 */
        case 4:  return (seq_t)2;          /* n≡4 mod 16 */
        case 8:  return (seq_t)4;          /* n≡8 mod 16 */
        case 12: return (seq_t)1;          /* n≡12 mod 16 */
        default: return (seq_t)0;          /* unreachable */
    }
}
