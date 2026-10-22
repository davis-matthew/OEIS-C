/*
 * A302141  Multiplicative order of 16 mod 2n+1.
 * https://oeis.org/A302141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of 16 modulo (2n+1)
 * Range:     u8 n <= 259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, calc_phi, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The order is obtained by reducing φ(2n+1) using prime divisors. */
/* a(n) = 1 for n = 0, and equals the minimal k>0 with 16^k ≡ 1 (mod 2n+1). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/calc_phi.h"
#include "../utils/build_primes.h"

static seq_t _order16_mod(seq_t m)
{
    /* Special case m = 1 (n = 0) */
    if (m == (seq_t)1)
        return (seq_t)1;          /* by definition a(0) = 1 */

    /* φ(m) -- guaranteed ≤ m-1 <= 20000 for n ≤ 10000 */
    seq_t phi_seq = calc_phi(m);
    uint64_t phi = (uint64_t)phi_seq;

    /* Sieve primes up to φ(m) for factorisation. */
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;
    if (!primes) {
        idx_t limit = (idx_t)20000;    /* φ(m) ≤ m-1 < 20001 */
        build_primes(limit, &primes, &prime_cnt);
    }

    uint64_t order = phi;          /* current candidate order */
    uint64_t temp = phi;           /* copy of φ(m) for factoring */

    /* Reduce the order by prime factors of φ(m) */
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (temp % p == 0) {
            while (temp % p == 0)
                temp /= p;                  /* strip factor from φ(m) */
            while (order % p == 0) {
                if (mod_pow((seq_t)16, order / p, m) == (seq_t)1)
                    order /= p;
                else
                    break;
            }
        }
    }
    if (temp > 1) {                   /* remaining prime factor */
        uint64_t p = temp;
        while (order % p == 0) {
            if (mod_pow((seq_t)16, order / p, m) == (seq_t)1)
                order /= p;
            else
                break;
        }
    }

    return (seq_t)order;   /* this is already the desired sequence value */
}

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * (seq_t)2 + (seq_t)1;   /* m = 2n + 1 */
    return _order16_mod(m);
}
