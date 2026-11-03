/*
 * A261371  Prime numbers that sum to a prime when added to their digital
 *          root.
 * https://oeis.org/A261371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261371: primes p such that p + digital_root(p) is prime. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

static seq_t *prop_primes = NULL;
static size_t prop_cap   = 0;
static size_t prop_cnt   = 0;
static seq_t   cand      = 2;

/* Ensure enough capacity for the property prime list */
static void grow_prop(void) {
    size_t new_cap = prop_cap == 0 ? 16 : prop_cap << 1;
    seq_t *tmp = mem_resize(seq_t, prop_primes, new_cap);
    if (!tmp) abort();            /* allocation failed */
    prop_primes = tmp;
    prop_cap    = new_cap;
}

/* Find the next prime p such that p + digital_root(p) is prime
   and store it in prop_primes.  cand holds the next candidate
   to test. */
static void find_next_property_prime(void) {
    for (;;) {
        seq_t p;
        /* Find the next prime starting from cand */
        if (cand <= 2) {
            p = 2;
            cand = 3;                 /* next candidate will be 3 (odd) */
        } else {
            p = cand;
            if ((p & 1) == 0) p += 1; /* ensure we start with an odd number */
            while (!is_prime_big(p)) {
                p += 2;               /* skip even numbers */
            }
            cand = p + 2;             /* next trial starts after this prime */
        }

        /* Compute digital root: 1 + ((p-1) % 9) */
        seq_t dr  = ((p - 1) % 9) + 1;
        seq_t sum = p + dr;
        if (is_prime_big(sum)) {
            if (prop_cnt >= prop_cap) grow_prop();
            prop_primes[prop_cnt++] = p;
            break;                    /* we found a property prime */
        }
        /* otherwise continue searching for the next prime */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for n < 1 */
    while (prop_cnt < (size_t)n) {
        find_next_property_prime();        /* generate more terms as needed */
    }
    return prop_primes[(size_t)n - 1];
}
