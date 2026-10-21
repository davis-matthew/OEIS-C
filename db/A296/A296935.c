/*
 * A296935  Rational primes that decompose in the field Q(sqrt(11)).
 * https://oeis.org/A296935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th rational prime that splits in Q(√11), * i.e.
 *            the n‑th prime p with (11|p)=1 (Legendre symbol). * The function
 *            uses a simple incremental sieve: it tests successive * odd
 *            numbers for primality (via is_prime) and checks the Legendre *
 *            symbol by modular exponentiation
 * Range:     u8 n <= 24, u16 n <= 3234, u32 n <= 9973, u64 n <= 9973,
 *            u128 n <= 9973, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"    /* bool is_prime(idx_t x); */

static inline uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1U)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1U;
    }
    return result;
}

static inline bool legendre_11_split(uint64_t p)
{
    /* (11|p) = 1  iff 11^((p-1)/2) ≡ 1 (mod p)  for odd prime p>2. */
    if (p == 2)        return false;   /* ramified */
    if (p == 11)       return false;   /* ramified  */
    return mod_pow64(11, (p - 1) / 2, p) == 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, return 0 */

    /* Static state to keep the list of already found split primes */
    static uint64_t cur_candidate = 3;   /* next odd number to test   */
    static size_t splits_cnt = 0;        /* number of split primes found so far */
    static seq_t *splits = NULL;        /* array of split primes       */
    static size_t splits_cap = 0;        /* capacity of the array       */

    /* Ensure we have at least n split primes stored */
    while (splits_cnt < (size_t)n) {

        /* Test primality of the current candidate */
        if (is_prime(cur_candidate)) {
            if (legendre_11_split(cur_candidate)) {
                /* Grow array if necessary */
                if (splits_cnt + 1 > splits_cap) {
                    size_t new_cap = splits_cap ? splits_cap << 1U : 16;
                    while (new_cap <= splits_cnt + 1) new_cap <<= 1U;
                    seq_t *tmp = (seq_t *)realloc(splits, new_cap * sizeof(seq_t));
                    splits = tmp;
                    splits_cap = new_cap;
                }
                splits[splits_cnt++] = (seq_t)cur_candidate;
            }
        }

        /* Advance to the next odd candidate */
        cur_candidate += 2;
    }

    /* Return the n‑th split prime (1‑based). */
    return splits[n - 1];
}
