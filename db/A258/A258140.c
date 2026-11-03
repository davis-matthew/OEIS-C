/*
 * A258140  Number of ways to write 6*n + 2 as p^2 + q with p and q both
 *          prime.
 * https://oeis.org/A258140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write 6*n+2 as p^2 + q with p and q
 *            primes. Search over primes p <= sqrt(6*n+2), count each q =
 *            6*n+2 - p^2 that is prime
 * Range:     u8 n <= 10, u16 n <= 1822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9723 (2026-11-03)
 * Helpers:   build_primes_upto, ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
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

seq_t a(idx_t n)
{
    /* 6*n + 2 as seq_t to avoid overflow of intermediate 64‑bit multiplication */
    seq_t num = (seq_t)n * 6 + 2;

    /* sqrt_num = floor(sqrt(num)) */
    seq_t sqrt_num = floor_sqrt(num);
    idx_t limit = (idx_t)sqrt_num;

    /* static cache of primes up to the largest limit requested */
    static idx_t primes_limit = 0;
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;

    if (limit > primes_limit) {
        if (primes) {
            free(primes);
            primes = NULL;
        }
        build_primes_upto(limit, &primes, &primes_cnt);
        primes_limit = limit;
    }

    seq_t count = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t p_sq = (seq_t)p * (seq_t)p;
        if (p_sq > num) {
            break;
        }
        seq_t q = num - p_sq;
        if (is_prime_big(q)) {
            ++count;
        }
    }
    return count;
}
