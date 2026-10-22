/*
 * A373480  a(n) = A003415(n) mod A001414(n), where A003415 is the arithmetic
 *          derivative, and A001414 is the sum of prime factors with
 *          multiplicity.
 * https://oeis.org/A373480
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A003415(n) mod A001414(n)
 * Range:     u8 n <= 1075, u16 n <= 9397, u32 n <= 9397, u64 n <= 9397,
 *            u128 n <= 9397, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* A003415  : arithmetic derivative of n
   A001414  : sum of prime factors of n counted with multiplicity */

seq_t a(idx_t n)
{
    /* defined only for n >= 2; for smaller indices return 0 for safety */
    if (n < 2)
        return (seq_t)0;

    uint64_t nn   = (uint64_t)n;
    uint64_t temp = nn;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt   = 0;

    /* factor 2 */
    if ((temp & 1ULL) == 0ULL) {
        uint64_t e = 0;
        while ((temp & 1ULL) == 0ULL) {
            temp >>= 1;
            ++e;
        }
        primes[cnt] = 2;
        exps[cnt]   = e;
        ++cnt;
    }

    /* odd factors */
    for (uint64_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            uint64_t e = 0;
            while (temp % d == 0) {
                temp /= d;
                ++e;
            }
            primes[cnt] = d;
            exps[cnt]   = e;
            ++cnt;
        }
    }

    /* remaining prime factor */
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* compute A001414(n) = sum of prime factors with multiplicity */
    uint64_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += primes[i] * exps[i];

    if (sum == 0)                /* impossible for n>=2, but keep safety */
        return (seq_t)0;

    /* compute the derivative modulo the sum */
    uint64_t deriv_mod = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t term = nn / primes[i];          /* n/p_i */
        uint64_t e    = exps[i];
        /* term_mod = (term * e) mod sum, using 128‑bit arithmetic to avoid overflow */
        unsigned __int128 prod = (unsigned __int128)(term % sum) * (e % sum);
        uint64_t term_mod = (uint64_t)(prod % sum);
        deriv_mod = deriv_mod + term_mod;
        if (deriv_mod >= sum)
            deriv_mod -= sum;
    }

    return (seq_t)deriv_mod;
}
