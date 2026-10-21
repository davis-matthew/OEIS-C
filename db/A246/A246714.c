/*
 * A246714  Catalan(n) mod prime(n).
 * https://oeis.org/A246714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 5185, u32 n <= 7288, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Catalan(n) = binom(2n,n) - binom(2n,n+1) */
#include <stdint.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

static seq_t binom_mod(idx_t a, idx_t k, seq_t mod)
{
    if (k > a) return 0;
    /* work with the smaller of k and a-k for fewer iterations */
    if (k > a - k) k = a - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(a - k + i);
        res = (res * num) % mod;
        seq_t inv = mod_pow((seq_t)i, (uint64_t)mod - 2, mod);
        res = (res * inv) % mod;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t mod = get_prime(n);
    idx_t two_n = 2 * n;

    seq_t c1 = binom_mod(two_n, n, mod);      /* binom(2n,n) modulo P */
    seq_t c2 = binom_mod(two_n, n + 1, mod);  /* binom(2n,n+1) modulo P */

    seq_t res = (c1 >= c2) ? (c1 - c2) : (c1 + mod - c2);
    return res % mod;
}
