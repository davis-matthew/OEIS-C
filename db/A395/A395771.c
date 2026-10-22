/*
 * A395771  The twisted Euler phi-function for the Dirichlet character
 *          kronecker(-7,.) mod 7.
 * https://oeis.org/A395771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(1)=1; for n>1 factorize n=∏p^e; if p==7 set term=p^e; if
 *            p≡1,2,4 (mod 7) set term=p^(e-1)*(p-1); otherwise (p≡3,5,6) set
 *            term=p^(e-1)*(p+1); a(n)=∏term
 * Range:     u8 n <= 194, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Computes the twisted Euler phi-function for the Kronecker character (‑7/·) mod 7. */
seq_t a(idx_t n)
{
    if (n <= 1u)          /* a(1)=1, a(0) is not defined but handled for safety */
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t tmp   = (uint64_t)n;   /* tmp is used for factorization */

    /* Special handling of prime 7: a(7^e)=7^e */
    while (tmp % 7 == 0) {
        tmp /= 7;
        result *= (seq_t)7;
    }

    /* Factor remaining part (only primes ≠ 7) */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p != 0)
            continue;

        /* Count exponent e of prime p */
        uint64_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }

        /* Compute term p^(e‑1) * (p‑1) or (p+1) depending on p mod 7 */
        seq_t base_pow = (seq_t)1;
        for (uint64_t k = 0; k < e - 1; ++k)
            base_pow *= (seq_t)p;

        if (p % 7 == 1 || p % 7 == 2 || p % 7 == 4) {
            result *= base_pow * (seq_t)(p - 1);
        } else {
            result *= base_pow * (seq_t)(p + 1);
        }
    }

    /* If tmp > 1, it is the remaining prime factor (≠ 7) */
    if (tmp > 1) {
        uint64_t p = tmp;   /* single prime factor left */
        /* exponent e is 1, so base_pow = 1 */
        if (p % 7 == 1 || p % 7 == 2 || p % 7 == 4)
            result *= (seq_t)(p - 1);
        else
            result *= (seq_t)(p + 1);
    }

    return result;
}
