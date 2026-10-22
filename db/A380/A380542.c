/*
 * A380542  a(n) is the multiplicative order of -9 modulo prime(n); a(2) = 0
 *          for completion.
 * https://oeis.org/A380542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = multiplicative order of -9 modulo prime(n)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0; /* prime 3 gives -9 ≡ 0 mod 3 */

    /* nth prime */
    uint32_t p = get_prime(n);
    if (p == 3) return (seq_t)0;

    uint64_t mod = p;
    uint64_t g = (mod - (9 % mod)) % mod;   /* -9 mod p */

    uint64_t ord = mod - 1;                 /* p-1 */

    /* factorise p-1 */
    uint64_t temp = ord;
    uint32_t factors[64];
    size_t nf = 0;
    for (uint64_t i = 2; i * i <= temp; ++i) {
        if (temp % i == 0) {
            factors[nf++] = (uint32_t)i;
            while (temp % i == 0) temp /= i;
        }
    }
    if (temp > 1) {
        factors[nf++] = (uint32_t)temp;
    }

    /* reduce ord by prime factors */
    for (size_t j = 0; j < nf; ++j) {
        uint32_t q = factors[j];
        while (ord % q == 0) {
            uint64_t cand = ord / q;
            seq_t val = mod_pow((seq_t)g, cand, (seq_t)mod);
            if (val == (seq_t)1)
                ord = cand;
            else
                break;
        }
    }

    return (seq_t)ord;
}
