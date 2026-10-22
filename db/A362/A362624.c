/*
 * A362624  a(n) = Sum_{d|n, gcd(d,n/d)=1} psi(d), where psi is the Dedekind
 *          psi function (A001615).
 * https://oeis.org/A362624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Product_{p^e||n} (1 + (p+1) p^{e-1})
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is multiplicative, with a(p^e) = 1 + (p+1)p^{e-1}. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t result = 1;
    uint64_t tmp = (uint64_t)n;
    uint64_t p;
    int e;

    /* handle factor 2 separately for speed */
    if ((tmp & 1ULL) == 0) {
        e = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            e++;
        }
        p = 2;
        seq_t p_pow = 1;
        for (int k = 1; k < e; ++k)
            p_pow *= (seq_t)p;
        seq_t term = (seq_t)1 + ((seq_t)(p + 1)) * p_pow;
        result *= term;
    }

    /* iterate odd factors */
    for (uint64_t i = 3; i * i <= tmp; i += 2) {
        if (tmp % i == 0) {
            e = 0;
            while (tmp % i == 0) {
                tmp /= i;
                e++;
            }
            p = i;
            seq_t p_pow = 1;
            for (int k = 1; k < e; ++k)
                p_pow *= (seq_t)p;
            seq_t term = (seq_t)1 + ((seq_t)(p + 1)) * p_pow;
            result *= term;
        }
    }

    if (tmp > 1) { /* remaining prime factor */
        p = tmp;
        e = 1;
        seq_t p_pow = 1; /* p^{e-1} = 1 */
        seq_t term = (seq_t)1 + ((seq_t)(p + 1)) * p_pow;
        result *= term;
    }

    return result;
}
