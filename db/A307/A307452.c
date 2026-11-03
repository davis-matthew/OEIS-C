/*
 * A307452  Primes p such that the sum of the decimal digits of p^4 is also a
 *          prime.
 * https://oeis.org/A307452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 23, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   build_primes_upto, digit_sum, ipow_idx, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the nth prime p such that the sum of the decimal digits of p⁴ is prime. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/digit_sum.h"
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
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

seq_t a(idx_t n)
{
    /* Handle the offset (a(1) is the first term). */
    if (n == 0) return (seq_t)0;

    /* Static cache of qualifying primes */
    static seq_t *qual_primes = NULL;
    static size_t qual_cap   = 0;
    static size_t qual_size  = 0;

    /* Static data for prime generation */
    static uint64_t *prime_arr   = NULL;
    static size_t   prime_cnt   = 0;
    static size_t   prime_idx   = 0;      /* next prime to process */
    static idx_t    sieve_limit = 0;      /* highest limit already sieved */

    /* If we already have enough terms, return the cached one. */
    if (qual_size >= (size_t)n) return qual_primes[(size_t)n - 1];

    /* Grow the sieve until we have at least n qualifying primes. */
    idx_t target = (sieve_limit == 0) ? 1000 : sieve_limit * 2;
    while (qual_size < (size_t)n) {
        if (target <= sieve_limit + 100) target = sieve_limit + 100;

        /* Build primes up to the new target. */
        if (prime_arr != NULL) free(prime_arr);   /* free previous buffer */
        build_primes_upto(target, &prime_arr, &prime_cnt);

        /* Process newly discovered primes. */
        for (size_t i = prime_idx; i < prime_cnt; ++i) {
            uint64_t p = prime_arr[i];
            seq_t p4   = ipow_idx((seq_t)p, 4);
            seq_t sum  = digit_sum(p4);

            if (is_prime_big(sum)) {
                /* Grow the qualifying prime array if needed. */
                if (qual_size == qual_cap)
                    grow_seq(&qual_primes, &qual_cap, qual_size + 1);
                qual_primes[qual_size++] = (seq_t)p;
            }
        }

        /* Update bookkeeping for the next iteration. */
        prime_idx   = prime_cnt;
        sieve_limit = target;
        target      = sieve_limit * 2;
    }

    return qual_primes[(size_t)n - 1];
}
