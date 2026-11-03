/*
 * A247948  Five-digit odd semiprimes with all digits distinct.
 * https://oeis.org/A247948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3175, u32 n <= 4858, u64 n <= 4858,
 *            u128 n <= 4858, big any n
 * Validated: full, pass, 4858 terms through n = 4858 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the n-th five‑digit odd semiprime with all digits distinct.
    This is done by generating the complete list once and returning the
    requested element.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Test whether all decimal digits of a 5‑digit integer are different. */
static inline bool distinct_digits(uint64_t v)
{
    bool used[10] = { false };
    while (v) {
        int d = v % 10;
        if (used[d])
            return false;
        used[d] = true;
        v /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Static storage for the sequence. */
    static seq_t *seq = NULL;   /* array of values                      */
    static size_t sz  = 0;      /* current number of stored values      */
    static size_t cap = 0;      /* capacity of the allocated array      */

    /* Build the sequence once.  */
    if (seq == NULL) {
        /* Small primes up to sqrt(99999) = 316. */
        size_t prime_cnt = 0;
        uint64_t *primes = NULL;
        build_primes_upto((idx_t)316, &primes, &prime_cnt);

        /* Scan all five‑digit numbers. */
        for (uint64_t x = 10000; x <= 99999; ++x) {
            if ((x & 1u) == 0)                       /* skip even numbers */
                continue;
            if (!distinct_digits(x))
                continue;

            /* Find the smallest prime factor of x. */
            uint64_t spf = 0;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = primes[i];
                if (p * p > x)
                    break;
                if (x % p == 0) {
                    spf = p;
                    break;
                }
            }
            if (spf == 0)               /* x is prime, not semiprime */
                continue;

            uint64_t q = x / spf;
            if (!is_prime_big((seq_t)q))
                continue;               /* not semiprime */

            /* Store the value. */
            append_candidate(&seq, &sz, &cap, (seq_t)x);
        }

        /* The primes array was allocated by build_primes_upto; free it. */
        free(primes);
    }

    if (n < 1)
        return 0;
    size_t idx = (size_t)(n - 1);
    if (idx >= sz)
        return 0;                      /* out of bounds (should not happen) */
    return seq[idx];
}
