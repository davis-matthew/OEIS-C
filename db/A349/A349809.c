/*
 * A349809  a(n) = n^2 - A099611(n).
 * https://oeis.org/A349809
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 - largest odd semiprime < n^2 Sequence A349809
 * Range:     u8 n <= 8, u16 n <= 89, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9968 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
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

static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Build a list of odd primes up to 'limit' */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;           /* already done */

    /* Allocate a sieve array */
    size_t sieve_size = (size_t)(limit + 1);
    uint8_t *sieve = (uint8_t*)malloc(sieve_size);
    if (!sieve) exit(1);                         /* memory error */
    memset(sieve, 0, sieve_size);
    sieve[0] = sieve[1] = 1;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                sieve[j] = 1;
            }
        }
    }

    /* Count odd primes to allocate list */
    size_t list_size = (size_t)(limit / 2 + 10);
    idx_t *new_list = (idx_t*)malloc(list_size * sizeof(idx_t));
    if (!new_list) exit(1);

    size_t cnt = 0;
    for (idx_t i = 3; i <= limit; i += 2) {
        if (!sieve[i]) {
            if (cnt < list_size) {
                new_list[cnt++] = i;
            }
        }
    }

    free(sieve);
    free(prime_list);
    prime_list = new_list;
    prime_count = cnt;
    prime_limit = limit;
}

/* Test if x is an odd semiprime (product of two odd primes) */
static bool is_odd_semiprime(seq_t x)
{
    if ((x & 1) == 0) return false;   /* even numbers are not odd semiprimes */

    seq_t sqrt_x = floor_sqrt(x);
    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = (seq_t)prime_list[i];
        if (p > sqrt_x) break;
        seq_t rem = x % p;
        if (rem == 0) {
            seq_t q = x / p;
            return is_prime_big(q);     /* both factors must be prime */
        }
    }
    return false;   /* no prime factor found */
}

seq_t a(idx_t n)
{
    /* Ensure we have all odd primes up to n for factorization */
    ensure_primes(n);

    seq_t N = (seq_t)n * (seq_t)n;
    seq_t cand = N - 1;                     /* start just below n^2 */
    if ((cand & 1) == 0) cand -= 1;         /* make odd */

    for (;;) {
        if (is_odd_semiprime(cand)) {
            return N - cand;
        }
        cand -= 2;                          /* next lower odd */
    }
}
