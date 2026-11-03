/*
 * A261627  Number of primes p such that n-(p*n'-1) and n+(p*n'-1) are both
 *          prime, where n' is 1 or 2 according as n is odd or even.
 * https://oeis.org/A261627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 865, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9447 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A261627:
   a(n) = number of primes p such that n-(p*n'‑1) and n+(p*n'‑1) are both prime,
   where n' = 1 if n is odd, else 2. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
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

static uint64_t *prime_cache = NULL;
static size_t      prime_count = 0;
static idx_t       prime_limit = 0;

/* Ensure that all primes up to `limit` are available in `prime_cache`. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;
    free(prime_cache);
    build_primes_upto(limit, &prime_cache, &prime_count);
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* n' = 1 if n odd, 2 if n even */
    uint8_t np = (n & 1) ? 1 : 2;

    /* For odd n we need primes < n.
       For even n we need primes < n/2.
       We ask for primes up to `limit`, which may include one extra prime
       that we will skip when the left value becomes ≤ 1. */
    idx_t limit = (np == 1) ? n : (n >> 1);
    if (limit < 1) limit = 1;          /* avoid zero limit for n == 1 */

    ensure_primes(limit);

    seq_t count = 0;
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t p = prime_cache[i];

        /* left = n - (p*np - 1) = n - p*np + 1  */
        /* left <= 1 iff p*np >= n; compared first, since n - p*np wrapped
           around in an unsigned seq_t */
        if ((seq_t)p * (seq_t)np >= (seq_t)n) break;   /* further p only make left smaller */
        seq_t left = (seq_t)n - (seq_t)p * (seq_t)np + 1;

        /* right = n + (p*np - 1) */
        seq_t right = (seq_t)n + (seq_t)p * (seq_t)np - 1;

        if (is_prime_big(left) && is_prime_big(right))
            ++count;
    }
    return count;
}
