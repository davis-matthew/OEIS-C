/*
 * A242752  Primes p such that pi(p) is a primitive root modulo p, where pi(p)
 *          is the number of primes not exceeding p.
 * https://oeis.org/A242752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computes the n-th prime p such that pi(p) is a primitive root
 *            modulo p
 * Range:     u8 n <= 21, u16 n <= 2432, u32 n <= 3669, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* https://oeis.org/A242752 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Small integer square root for uint64_t: returns ⌊√n⌋. */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = 3037000499uLL;          /* 2^32-1, sqrt(UINT64_MAX) */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            if (mid == 0xFFFFFFFFuLL) return mid;  /* avoid overflow */
            low = mid + 1;
        } else {
            if (mid == 0) return 0;
            high = mid - 1;
        }
    }
    return high;
}

/* Modular exponentiation for seq_t (built‑in or big‑int). */
static seq_t mod_pow_seq(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        exp >>= 1;
        if (exp) base = (base * base) % mod;
    }
    return result;
}

/* Test whether n < 2^64 prime candidate is prime by trial division. */
static inline bool is_prime_candidate(uint64_t n, const uint64_t *primes, size_t plen)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    uint64_t limit = isqrt_u64(n);
    for (size_t i = 0; i < plen; ++i) {
        uint64_t p = primes[i];
        if (p > limit) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Checks whether pi is a primitive root modulo p. */
static bool is_primitive_root(seq_t pi, seq_t p, const uint64_t *primes, size_t plen)
{
    seq_t phi = p - 1;
    seq_t temp = phi;

    for (size_t i = 0; i < plen; ++i) {
        uint64_t q = primes[i];
        seq_t q_seq = (seq_t)q;
        if (q_seq * q_seq > temp) break;
        if (temp % q_seq == 0) {
            seq_t exp = phi / q_seq;
            if (mod_pow_seq(pi, exp, p) == 1) return false;
            while (temp % q_seq == 0) temp /= q_seq;
        }
    }
    if (temp > 1) {                                   /* remaining prime factor */
        seq_t exp = phi / temp;
        if (mod_pow_seq(pi, exp, p) == 1) return false;
    }
    return true;
}

/* The required function. */
seq_t a(idx_t n)
{
    /* static data: primes and sequence cache */
    static uint64_t *prime_list = NULL;
    static size_t prime_len = 0, prime_cap = 0;
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static uint64_t candidate = 3;          /* next odd number to test */
    static bool initialized = false;

    if (!initialized) {
        /* initialise prime list with 2 */
        prime_cap = 4;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_len = 1;
        prime_list[0] = 2;

        /* initialise sequence array with first term 2 */
        seq_cap = 4;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 1;
        seq[0] = (seq_t)2;

        initialized = true;
    }

    /* generate further terms until we have n of them */
    while (seq_len < (size_t)n) {
        /* find next prime */
        while (!is_prime_candidate(candidate, prime_list, prime_len)) {
            candidate += 2;          /* skip even numbers */
        }

        /* store new prime */
        if (prime_len == prime_cap) {
            prime_cap *= 2;
            prime_list = (uint64_t *)realloc(prime_list,
                                            prime_cap * sizeof(uint64_t));
        }
        prime_list[prime_len++] = candidate;

        /* pi(p) = prime_len   (since we just added the p-th prime) */
        seq_t pi_seq = (seq_t)prime_len;

        /* check primitive‑root condition */
        if (is_primitive_root(pi_seq, (seq_t)candidate,
                              prime_list, prime_len)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t *)realloc(seq,
                                       seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)candidate;
        }

        candidate += 2;               /* next odd to test */
    }

    return seq[(size_t)n - 1];
}
