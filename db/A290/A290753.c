/*
 * A290753  Semiprimes beginning with 1 such that deleting the 1 leaves a
 *          prime.
 * https://oeis.org/A290753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer of the form 1p where p is prime, * 1p is
 *            a semiprime (product of two primes)
 * Range:     u8 n <= 10, u16 n <= 465, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9816 (2026-11-03)
 * Helpers:   build_primes_upto, ipow_i32, is_prime_seq, is_prime_u64, mem, next_prime, num_digits_idx, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function iterates over primes p, constructs N = 10^digits(p)+p,
 * checks whether N is a semiprime, and stores the qualifying
 * numbers in a cache. It works for any seq_t and idx_t supplied by the
 * build (8, 16, 32, 64 or 128 bit values, or arbitrary‑precision).
 * All required utilities are included from the ../utils directory. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_seq.h"
#include "../utils/next_prime.h"
#include "../utils/num_digits_idx.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

static bool is_semiprime(seq_t N)
{
    /* Prime test */
    if (is_prime_big(N))
        return false;

    /* If N fits in uint64_t, use fast trial division */
    if ((seq_t)UINT64_MAX >= N) {
        uint64_t n64 = (uint64_t)N;
        uint64_t d = smallest_prime_factor(n64);
        if (d == n64)     /* in principle unreachable because N is composite */
            return false;
        uint64_t other = n64 / d;
        return is_prime_seq(other);
    }

    /* In general: trial division with small primes up to sqrt(N) */
    seq_t sqrtN = floor_sqrt(N);
    idx_t limit = (idx_t)sqrtN;            /* sqrtN fits into idx_t (see analysis) */

    /* Static storage for primes up to current limit */
    static uint64_t *primes = NULL;
    static size_t primes_len = 0;
    static idx_t primes_limit = 0;

    if (limit > primes_limit) {
        /* Rebuild prime list up to new limit */
        free(primes);
        build_primes_upto(limit, &primes, &primes_len);
        primes_limit = limit;
    }

    /* Trial division */
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        seq_t pp = (seq_t)p * (seq_t)p;
        if (pp > N)
            break;
        if (N % (seq_t)p == 0) {
            seq_t other = N / (seq_t)p;
            return is_prime_big(other);
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Ensure cache can hold at least n elements */
    while (cache_len < (size_t)n) {
        /* Next prime p */
        static uint64_t current_p = 1;
        current_p = next_prime(current_p + 1);

        int k = num_digits_idx((idx_t)current_p);
        seq_t pow10k = pow10(k);
        seq_t candidate = pow10k + (seq_t)current_p;

        if (is_semiprime(candidate)) {
            /* Expand cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = candidate;
        }
    }
    return cache[n - 1];
}
