/*
 * A353473  a(n) = 1 if n is a number of the form p * q^2, where p and q are
 *          primes with p < q, otherwise 0.
 * https://oeis.org/A353473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9644 (2026-11-03)
 * Helpers:   build_primes_upto, ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353473
 * a(n) = 1 if n is of the form p * q^2 with primes p < q, otherwise 0.
 * (OEIS: a(n) = 1 if there exist distinct primes p<q with n = p*q^2)
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper function headers */
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u64.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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

/* Static cache of primes up to the current limit */
static uint64_t *prime_cache = NULL;   /* array of primes */
static size_t    prime_cnt   = 0;      /* number of primes stored */
static idx_t     prime_limit = 0;      /* largest prime currently cached */

/* Rebuild the prime cache up to a new limit */
static void rebuild_prime_cache(idx_t new_limit)
{
    free(prime_cache);
    build_primes_upto(new_limit, &prime_cache, &prime_cnt);
    prime_limit = new_limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* The smallest possible product p * q^2 with p < q is 2 * 3^2 = 18 */
    if (n < (idx_t)18)
        return (seq_t)0;

    /* Integer square root of n (as seq_t) */
    seq_t sqrt_n = floor_sqrt((seq_t)n);
    idx_t lim = (idx_t)sqrt_n;   /* lim fits in idx_t because n <= 2^64-1 */

    /* Ensure the prime cache contains all primes up to lim */
    if (lim > prime_limit)
        rebuild_prime_cache(lim);

    /* Scan primes q up to sqrt(n) */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t q   = prime_cache[i];
        seq_t    q2  = (seq_t)q * (seq_t)q;   /* q^2 */

        if (q2 > (seq_t)n)
            break;

        if ((seq_t)n % q2 != 0)
            continue;

        seq_t m = (seq_t)n / q2;  /* Candidate prime p */

        if (m < (seq_t)q && is_prime_big(m))
            return (seq_t)1;
    }

    return (seq_t)0;
}
