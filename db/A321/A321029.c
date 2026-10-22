/*
 * A321029  Number of integers x such that 1 <= x <= n and gcd(x,n) =
 *          gcd(x+4,n) = gcd(x+6,n) = gcd(x+10,n) = gcd(x+12,n) = 1.
 * https://oeis.org/A321029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A321029: a(n) = ∏_{p^e‖n} f(p,e)   with
   f(p,e) = p^(e-1)              if p <= 5
   f(p,e) = (p-5)*p^(e-1)        if p >= 7
   (for e=0 we omit the factor; a(1)=1) */

#include <stdint.h>
#include <stddef.h>

/* seq_t and idx_t are provided by the build */
seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1)=1, a(0)=0 (ignored) */
        return (n == 1) ? (seq_t)1 : (seq_t)0;

    /* factor n by trial division – n ≤ 10000 in tests, so this is fast */
    const size_t MAX_FACTORS = 20;
    uint64_t prime[MAX_FACTORS];
    int exp[MAX_FACTORS];
    size_t cnt = 0;

    idx_t temp = n;
    /* factor 2 separately */
    if ((temp & 1ULL) == 0) {
        int e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++e;
        }
        prime[cnt] = 2;
        exp[cnt] = e;
        ++cnt;
    }

    /* odd factors up to sqrt(temp) */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (cnt < MAX_FACTORS) {
                prime[cnt] = (uint64_t)p;
                exp[cnt] = e;
                ++cnt;
            }
        }
    }
    /* remaining prime > sqrt(original n) */
    if (temp > 1) {
        if (cnt < MAX_FACTORS) {
            prime[cnt] = (uint64_t)temp;
            exp[cnt] = 1;
            ++cnt;
        }
    }

    /* compute multiplicative value */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = prime[i];
        int e = exp[i];

        /* compute p^(e-1) */
        seq_t term = (seq_t)1;
        for (int k = 0; k < e - 1; ++k)
            term *= (seq_t)p;

        if (p >= 7)          /* add the (p-5) factor if needed */
            term *= (seq_t)(p - 5);

        result *= term;
    }

    return result;
}
