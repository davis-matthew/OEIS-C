/*
 * A276731  Number of solutions to y^2 + y == x^3 - 7 (mod p) as p runs
 *          through the primes.
 * https://oeis.org/A276731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6501, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n): number of solutions (x,y) modulo prime(n) of
 *  y^2 + y ≡ x^3 – 7 (mod prime(n)).  */
#include <stdint.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */

    uint32_t p = get_prime(n);               /* prime(n) */
    if (p == 2U) {                           /* special handling for p = 2 */
        /* Brute‑force the 4 residue classes */
        seq_t cnt = 0;
        for (uint32_t x = 0; x < 2; ++x)
            for (uint32_t y = 0; y < 2; ++y)
                if (((y * y + y) % 2ULL) == ((x * x * x - 7ULL) % 2ULL))
                    ++cnt;
        return cnt;
    }

    /* For odd primes use quadratic‑residue test */
    seq_t count = 0;
    uint64_t mod = (uint64_t)p;

    for (uint32_t x = 0; x < p; ++x) {
        /* x^3 mod p */
        uint64_t xx = (uint64_t)x * x % mod;      /* x^2 mod p */
        uint64_t xxx = xx * x % mod;              /* x^3 mod p */

        /* RHS = x^3 – 7 (mod p) */
        uint64_t rhs = (xxx + mod - (7 % mod)) % mod;

        /* Discriminant Δ = 1 + 4·rhs (mod p) */
        uint64_t D = (1 + 4 * rhs) % mod;

        /* Legendre symbol (D|p) via Jacobi (p is prime) */
        int j = jacobi_symbol(D, mod);

        if (j == 1)          /* two y solutions */
            count += (seq_t)2;
        else if (j == 0)     /* one y solution when Δ ≡ 0 */
            count += (seq_t)1;
    }
    return count;
}
