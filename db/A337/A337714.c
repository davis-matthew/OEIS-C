/*
 * A337714  Euler totient function phi(N) divided by the multiplicative order
 *          of 3 modulo N, with N = N(n) = floor((3*n-1)/2), for n >= 1.
 * https://oeis.org/A337714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(floor((3*n-1)/2)) / ord₃(floor((3*n-1)/2)) where φ is
 *            Euler’s totient and ord₃ is the multiplicative order of 3 modulo
 *            the argument
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   factorize, phi, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/factorize.h"
#include "../utils/powmod64.h"

/* Compute the multiplicative order of 3 modulo N (gcd(3,N)=1). */
static uint64_t order_of_3(uint64_t N)
{
    if (N <= 1) return 1;

    /* φ(N) fits in uint64_t for N up to 64‑bit */
    uint64_t phiN = (uint64_t)phi(N);
    uint64_t order = phiN;

    /* Factor φ(N) into its prime divisors. */
    uint64_t primes[64];
    int exp[64];
    size_t cnt = factorize(phiN, primes, exp);

    /* Reduce the order by each prime factor. */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        while (order % p == 0) {
            uint64_t cand = order / p;
            if (powmod64(3, cand, N) == 1)
                order = cand;
            else
                break;
        }
    }
    return order;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* N = floor((3*n-1)/2) – it always fits in 64‑bit for the usual limits. */
    uint64_t N = ( (uint64_t)3 * n - 1 ) / 2;

    seq_t phiN = phi(N);          /* φ(N) as seq_t */
    uint64_t ord = order_of_3(N);

    return phiN / (seq_t)ord;    /* exact division */
}
