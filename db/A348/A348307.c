/*
 * A348307  Primes p such that (p-1)/2, (p-2)/3, 2*p+1, 3*p+2 are all prime
 *          numbers.
 * https://oeis.org/A348307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A348307: primes p such that (p-1)/2, (p-2)/3, 2p+1, 3p+2 are all prime */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n) {
    /* Cache of found terms – 1‑indexed in the array */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Candidate q running over primes ≡ 2 (mod 3), i.e. safe primes q = (p‑1)/2 */
    static uint64_t next_q = 3; /* start from the first odd prime */

    if (n == 0)
        return (seq_t)0; /* out of range, but not used */

    while (cache_len < n) {
        /* Only consider q ≡ 2 (mod 3); otherwise p = 2q+1 would not satisfy (p‑2)/3 integer */
        if ((next_q % 3ULL) == 2ULL) {
            seq_t q = (seq_t)next_q;
            seq_t p = (q << 1) + 1;            /* p = 2q + 1 */
            if (is_prime_big(p)) {
                /* (p-1)/2 is already q (prime) */
                seq_t r = (p - 2) / 3;        /* (p-2)/3 */
                if (is_prime_big(r)) {
                    seq_t two_p1 = (p << 1) + 1;                 /* 2p + 1 */
                    if (is_prime_big(two_p1)) {
                        seq_t three_p2 = (p << 1) + p + 2;       /* 3p + 2 */
                        if (is_prime_big(three_p2)) {
                            /* Grow cache if necessary */
                            if (cache_len >= cache_cap) {
                                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                                cache = seq_realloc(cache, cache_cap, new_cap);
                                cache_cap = new_cap;
                            }
                            cache[cache_len++] = p;
                        }
                    }
                }
            }
        }
        /* Advance to the next prime q */
        next_q = next_prime(next_q + 1);
    }

    return cache[n - 1];
}
