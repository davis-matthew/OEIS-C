/*
 * A278083  a(n) is 1/6 of the number of primitive integral quadruples with
 *          sum = 2*m and sum of squares = 2*m^2, where m = 2*n-1.
 * https://oeis.org/A278083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(2n-1) where b(m) is multiplicative: * • b(1)=1 * • if m
 *            is odd and m = ∏ p_i^{k_i} (p_i odd primes) * b(p_i) = p_i+1 if
 *            p_i ≡ 3 (mod 4) * p_i-1 if p_i ≡ 1 (mod 4) * b(p_i^{k_i}) =
 *            p_i^{k_i-1} * b(p_i) * • b(m)=0 for even m * Hence a(n) = ∏
 *            p_i^{k_i-1} * b(p_i) for m=2n-1 (which is always...
 * Range:     u8 n <= 94, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = 2n-1 */
    uint64_t m = 2ULL * (uint64_t)n - 1ULL;

    /* m=1 ⇒ a(n)=1 */
    if (m <= 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    /* factor m */
    /* handle factor 2 – never occurs because m is odd, but keep for completeness */
    if (m % 2ULL == 0) {
        unsigned e = 0;
        while (m % 2ULL == 0) { m /= 2ULL; ++e; }
        seq_t powp = (seq_t)1;
        for (unsigned i = 0; i < e - 1; ++i) powp *= (seq_t)2;
        result *= powp; /* base for p=2 would be 0, but b(2)=0, so result stays 0 if any factor 2 */
        return (seq_t)0;
    }

    /* odd factors */
    for (uint64_t p = 3ULL; p * p <= m; p += 2ULL) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; ++e; }

            seq_t base;
            if (p % 4ULL == 1ULL)
                base = (seq_t)(p - 1ULL);
            else /* p ≡ 3 (mod 4) */
                base = (seq_t)(p + 1ULL);

            seq_t powp = (seq_t)1;
            for (unsigned i = 0; i < e - 1; ++i)
                powp *= (seq_t)p;

            result *= base * powp;
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1) {
        seq_t p = (seq_t)m;
        seq_t base = (p % 4ULL == 1ULL) ? (p - 1ULL) : (p + 1ULL);
        result *= base; /* exponent = 1 ⇒ powp = 1 */
    }

    return result;
}
