/*
 * A337923  a(n) is the exponent of the highest power of 2 dividing the n-th
 *          Fibonacci number.
 * https://oeis.org/A337923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of the highest power of 2 dividing Fibonacci(n).
 *            * Uses known congruence rules: * - if n ≡ 1,2 (mod 3) → a(n) = 0
 *            * - if n ≡ 3 (mod 6) → a(n) = 1 * - if n ≡ 6 (mod 12) → a(n) = 3
 *            * - if n ≡ 0 (mod 12) → a(n) = v₂(n) + 2 * * Here v₂(n) is the
 *            number of times 2 divides n (valuation...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial index before offset check. */
    if (n == 0) return 0;  /* Not used, but safe. */

    idx_t r12 = n % 12ULL;

    /* n ≡ 0 (mod 12): use valuation of n */
    if (r12 == 0ULL) {
        seq_t v = 0;
        /* count trailing zeros of n */
        idx_t tmp = n;
        while ((tmp & 1ULL) == 0ULL) {
            v++;
            tmp >>= 1ULL;
        }
        return v + 2;   /* v₂(n) + 2 */
    }

    /* n ≡ 6 (mod 12): exponent is 3 */
    if (r12 == 6ULL) {
        return 3;
    }

    /* n ≡ 3 (mod 6): exponent is 1 (covers residues 3 and 9 mod 12) */
    if ((n % 6ULL) == 3ULL) {
        return 1;
    }

    /* Remaining residues (1,2,4,5,7,8,10,11 mod 12) give exponent 0 */
    return 0;
}
