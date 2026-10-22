/*
 * A383287  a(n) = 1 if A276075(n) is a multiple of 4, otherwise 0, where
 *          A276075 is fully additive with a(p) = PrimePi(p)!.
 * https://oeis.org/A383287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if e2 + 2*(e3+e5) ≡ 0 (mod 4), else 0 where e2, e3, e5
 *            are the exponents of 2, 3, 5 in the prime factorisation of n.
 *            This follows from A276075(n) being additive with
 *            a(p)=PrimePi(p)!, and only primes 2,3,5 contribute modulo 4
 * Range:     u8 n <= 9449, u16 n <= 9449, u32 n <= 9449, u64 n <= 9449,
 *            u128 n <= 9449, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count exponents of 2, 3, 5 in n */
    uint64_t e2 = 0, e3 = 0, e5 = 0;
    uint64_t tmp = (uint64_t)n;   /* n fits in idx_t */
    while ((tmp & 1ULL) == 0) {   /* divisible by 2 */
        ++e2;
        tmp >>= 1;
    }
    while (tmp % 3ULL == 0) {
        ++e3;
        tmp /= 3ULL;
    }
    while (tmp % 5ULL == 0) {
        ++e5;
        tmp /= 5ULL;
    }
    /* Compute (e2 + 2*(e3+e5)) mod 4 */
    uint64_t sum = e2 + 2ULL * (e3 + e5);
    seq_t res = ((sum & 3ULL) == 0ULL) ? (seq_t)1 : (seq_t)0;
    return res;
}
