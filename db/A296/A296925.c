/*
 * A296925  Inert rational primes in the field Q(sqrt(-10)).
 * https://oeis.org/A296925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 3296, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9887 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A296925: primes congruent to 3,17,21,27,29,31,33,39 mod 40
   (primes for which (-10|p) = -1).
   Computes the sequence by testing successive odd numbers for primality
   and selecting those with the desired residues. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"
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

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    /* next odd integer to test (start from 3) */
    static uint64_t next_candidate = 3;

    if (n == 0)  /* OEIS offset starts at 1; return 0 for safety */
        return (seq_t)0;

    /* gather primes until we have at least n of them */
    while (cache_len < n) {
        /* test primality */
        if (is_prime_big((seq_t)next_candidate)) {
            uint64_t mod40 = next_candidate % 40ULL;
            if (mod40 == 3 || mod40 == 17 || mod40 == 21 ||
                mod40 == 27 || mod40 == 29 || mod40 == 31 ||
                mod40 == 33 || mod40 == 39) {

                /* grow cache if necessary */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    cache      = tmp;
                    cache_cap  = new_cap;
                }
                cache[cache_len++] = (seq_t)next_candidate;
            }
        }
        next_candidate += 2;   /* only odd numbers need be tested */
    }
    return cache[n - 1];
}
