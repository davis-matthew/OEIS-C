/*
 * A280895  For any n>=0, least prime k = p^d-2*n, where p is a prime and d is
 *          its number of digits.
 * https://oeis.org/A280895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least prime k such that k = p^d - 2*n, where p is a prime
 *            and d is the number of digits of p
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 59 terms through n = 950 (2026-11-03)
 * Helpers:   build_primes_upto, ipow, is_prime_u64, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow.h"
#include "../utils/num_digits0.h"
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

/* static cache for primes up to a limit */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_limit = 0;

/* Ensure we have a prime list covering at least up to prime_limit.
   If not allocated yet, build it once. */
static void ensure_prime_list(void)
{
    if (prime_list == NULL) {
        /* start with a moderate limit; it will be increased if needed */
        prime_limit = 2000000;          /* 2 million */
        build_primes_upto(prime_limit, &prime_list, &prime_count);
    }
}

seq_t a(idx_t n)
{
    /* coefficients in seq_t context */
    const seq_t two = (seq_t)2;

    ensure_prime_list();

    for (;;) { /* outer loop to allow limit extension if needed */
        for (size_t i = 0; i < prime_count; ++i) {
            const uint64_t p = prime_list[i];
            unsigned d = (unsigned)num_digits0(p);            /* d is small */
            seq_t power = ipow((seq_t)p, (seq_t)d);         /* p^d */
            seq_t two_n = two * (seq_t)n;                      /* 2*n */

            if (power <= two_n) continue;                     /* would underflow */

            seq_t candidate = power - two_n;
            if (candidate <= 1) continue;                     /* not prime */

            if (is_prime_big(candidate)) {
                return candidate;                             /* found the least prime */
            }
        }

        /* If we reach here, all primes up to the current limit failed.
           Double the limit and rebuild the prime list. */
        size_t old_limit = prime_limit;
        prime_limit *= 2;  /* double the search limit */
        uint64_t *new_list = NULL;
        size_t new_count = 0;

        build_primes_upto(prime_limit, &new_list, &new_count);
        /* free old list */
        free(prime_list);
        prime_list = new_list;
        prime_count = new_count;

        /* start scanning from the beginning again */
        (void)old_limit; /* unused; silence compiler warning */
    }
}
