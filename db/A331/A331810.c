/*
 * A331810  Smallest integer x such that prime(n) divides 1 + x + x^2 + ... +
 *          x^(q-1) where q = A023503(n), or 0 if no such x exists.
 * https://oeis.org/A331810
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   factor_u64, get_prime, largest_prime_divisor, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = the smallest integer x > 0 such that prime(n) | (1 + x + x^2 + ... + x^(q-1)),
   where q is the largest prime factor of prime(n)-1.  
   If no such x exists, a(n) = 0.              

   The implementation uses the fact that for any prime p > 2,
   the set of solutions modulo p to x^q ≡ 1 (x ≠ 1) is a cyclic subgroup
   of order q.  We find a primitive root g of the field modulo p, then
   compute g^t (with t = (p-1)/q) once and iterate its powers to find the
   smallest non‑trivial element in that subgroup.
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/largest_prime_divisor.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* offset: a(2) is first term */

    seq_t p = get_prime(n);                   /* large prime p   */
    uint64_t p64 = (uint64_t)p;

    /* largest prime factor of p-1  (q)  */
    uint64_t q = largest_prime_divisor(p64 - 1ULL);
    if (q <= 1) return 0;                    /* impossible, but be safe */

    uint64_t t = (p64 - 1ULL) / q;           /* (p-1)/q */

    /* factorize p-1 to get its distinct prime factors */
    size_t cnt = 0;
    uint64_t fac[64], exp[64];
    factor_u64(p64 - 1ULL, fac, exp, &cnt);  /* fac[i] are distinct primes */

    /* find a primitive root g of the field modulo p */
    seq_t g = 2;
    for (;; ++g) {
        bool ok = true;
        for (size_t i = 0; i < cnt && ok; ++i) {
            seq_t r = mod_pow(g, (p64 - 1ULL) / fac[i], p);
            if (r == 1) ok = false;
        }
        if (ok) break;                       /* g is a primitive root */
    }

    seq_t g_t = mod_pow(g, t, p);            /* g^t mod p   */

    seq_t min_val = 0;                       /* will store the answer */
    seq_t val = g_t;                         /* first non‑trivial member */
    if (min_val == 0 || val < min_val) min_val = val;

    for (uint64_t m = 2; m < q; ++m) {
        val = (val * g_t) % p;                /* next member of subgroup */
        if (val < min_val) min_val = val;
    }

    return min_val;                          /* zero only if no solution */
}
