/*
 * A369797  Denominator of the continued fraction
 *          1/(2-3/(3-4/(4-5/(...(n-1)-n/(n+2))))).
 * https://oeis.org/A369797
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A369797: Denominator of the continued fraction   
 * 1/(2-3/(3-4/(4-5/(...(n-1)-n/(n+2))))).
 * a(n) is the denominator of the reduced fraction 1/E2,
 * where E_k = k - (k+1)/E_{k+1} for k = n-2 … 2 and
 * E_{n-1} = (n-1) - n/(n+2).  The implementation
 * follows the recurrence  
 *   P_{k} = k*P_{k+1} - (k+1)*P_{k+2}
 * with P_{n} = n+2 and P_{n-1} = n^2-2.
 * a(n) = P_2 / gcd(P_2 , P_3).  Since a(n) divides 3n-2,
 * we compute all reductions modulo each prime power of
 * 3n-2 and combine the common factors by their exponents.
 * This avoids overflow for any 64‑bit implementation.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;                 /* undefined for n < 3 */

    /* 3n - 2 fits in 64 bits for all n of interest (n ≤ 10^4). */
    unsigned long long M = 3ull * n - 2ull;

    /* Prime factorisation of M (M ≤ 30000).  */
    unsigned long long primes[64];
    int exp[64];
    int pcnt = 0;
    unsigned long long tmp = M;

    for (unsigned long long p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            primes[pcnt] = p;
            exp[pcnt]   = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {                       /* remaining prime factor */
        primes[pcnt] = tmp;
        exp[pcnt]   = 1;
        ++pcnt;
    }

    /* G will accumulate the common divisor of P2 and P3 that
     * divides M (i.e. M / a(n)).  */
    unsigned long long G = 1;

    for (int fi = 0; fi < pcnt; ++fi) {
        unsigned long long p = primes[fi];
        int e = exp[fi];

        /* compute p^e */
        unsigned long long mod = 1;
        for (int k = 0; k < e; ++k) mod *= p;

        /* Initial values modulo mod */
        unsigned long long prev2 = (n + 2ull) % mod;                     /* P_n  */
        unsigned long long prev1 = ((unsigned long long)n * n - 2ull) % mod; /* P_{n-1} */

        /* Backward recurrence from k = n-2 downto 2 */
        long long kmax = (long long)n - 2;
        for (long long k = kmax; k >= 2; --k) {
            unsigned long long term1 = ( (unsigned long long)k * prev1 ) % mod;
            unsigned long long term2 = ( ((unsigned long long)k + 1ull) * prev2 ) % mod;
            unsigned long long new_val = (term1 + mod - term2) % mod;

            prev2 = prev1;
            prev1 = new_val;
        }

        unsigned long long P2_res = prev1 % mod;   /* P_2 mod mod */
        unsigned long long P3_res = prev2 % mod;   /* P_3 mod mod */

        /* exponent of p dividing P2 and P3 (up to e) */
        int vpP2 = 0;
        if (P2_res == 0) {
            vpP2 = e;
        } else {
            unsigned long long x = P2_res;
            while (x % p == 0) { x /= p; ++vpP2; }
        }

        int vpP3 = 0;
        if (P3_res == 0) {
            vpP3 = e;
        } else {
            unsigned long long x = P3_res;
            while (x % p == 0) { x /= p; ++vpP3; }
        }

        int vpG = vpP2 < vpP3 ? vpP2 : vpP3;
        unsigned long long powp = 1;
        for (int k = 0; k < vpG; ++k) powp *= p;
        G *= powp;                      /* G = ∏ p^{vpG}  */
    }

    unsigned long long result = M / G;
    return (seq_t)result;
}
