/*
 * A241043  Primes having primitive roots 2 and 3.
 * https://oeis.org/A241043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 952, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, grow_seq, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes p such that 2 and 3 are primitive roots modulo p.
   The function generates primes on demand, caches the results, and
   tests each prime by checking the multiplicative order of 2 and 3. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"
#include "../utils/grow_seq.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/is_prime.h"

/* Test whether 2 and 3 are primitive roots modulo a prime p > 3. */
static bool is_both_primitive_roots(uint64_t p, const uint64_t *primes,
                                    size_t primes_cnt)
{
    const uint64_t m = p - 1;
    uint64_t remaining = m;
    uint64_t sqrt_rem = _floor_isqrt_u64(remaining);

    for (size_t i = 0; i < primes_cnt && primes[i] <= sqrt_rem; ++i) {
        uint64_t q = primes[i];
        if (remaining % q == 0) {
            /* Test the primitive‑root condition for this factor. */
            if (mod_pow((seq_t)2, m / q, (seq_t)p) == (seq_t)1) return false;
            if (mod_pow((seq_t)3, m / q, (seq_t)p) == (seq_t)1) return false;
            while (remaining % q == 0) remaining /= q;
            sqrt_rem = _floor_isqrt_u64(remaining);
        }
    }
    if (remaining > 1) {
        /* remaining is a prime factor > sqrt(original). */
        if (mod_pow((seq_t)2, m / remaining, (seq_t)p) == (seq_t)1) return false;
        if (mod_pow((seq_t)3, m / remaining, (seq_t)p) == (seq_t)1) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Static cache of discovered terms. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;

    /* Prime list used for sieving and for factoring p‑1. */
    static uint64_t *prime_arr = NULL;
    static size_t prime_cnt = 0;
    static bool primes_inited = false;
    static size_t prime_index = 0;   /* next prime in prime_arr to test */

    /* Initialise the prime sieve once, enough for the expected limits. */
    if (!primes_inited) {
        build_primes_upto(1000000, &prime_arr, &prime_cnt);
        primes_inited = true;
    }

    /* Generate new terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        /* Scan previously sieved primes. */
        while (terms_len < (size_t)n && prime_index < prime_cnt) {
            uint64_t p = prime_arr[prime_index++];
            if (p == 2 || p == 3) continue;
            if (is_both_primitive_roots(p, prime_arr, prime_cnt)) {
                grow_seq(&terms, &terms_cap, terms_len + 1);
                terms[terms_len++] = (seq_t)p;
            }
        }

        /* If still not enough, search beyond the sieve limit. */
        if (terms_len < (size_t)n) {
            uint64_t candidate = prime_arr[prime_cnt - 1] + 2;
            while (terms_len < (size_t)n) {
                if (candidate < 2) { candidate = 5; continue; }
                if (is_prime((idx_t)candidate)) {
                    if (is_both_primitive_roots(candidate,
                                                prime_arr, prime_cnt)) {
                        grow_seq(&terms, &terms_cap, terms_len + 1);
                        terms[terms_len++] = (seq_t)candidate;
                    }
                }
                candidate += 2; /* skip even numbers */
            }
        }
    }

    return terms[n - 1];
}
