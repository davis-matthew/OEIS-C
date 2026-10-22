/*
 * A304203  If n = Product (p_j^k_j) then a(n) = Product (p_j^prime(k_j)).
 * https://oeis.org/A304203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^k‖n} p^prime(k) where prime(k) is the k‑th prime
 *            number (prime(1)=2)
 * Range:     u8 n <= 16, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1)=1 */

    uint64_t n64 = (uint64_t)n;

    /* Small buffers are enough for n up to huge values.
       The maximum number of distinct prime factors of a 64‑bit
       integer is < 15. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;

    factor_u64(n64, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p   = (seq_t)primes[i];
        seq_t exp = (seq_t)exps[i];

        /* prime(exp) → the exp‑th prime number */
        seq_t prime_exp = (seq_t)get_prime((idx_t)exp);

        /* p^prime(exp) */
        seq_t pow_val = big_pow(p, prime_exp);

        result *= pow_val;
    }

    return result;
}
