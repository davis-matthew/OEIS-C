/*
 * A364028  Odd squarefree semiprimes s = p*q such that p + q and p - q are
 *          not squarefree.
 * https://oeis.org/A364028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, is_squarefree, mem, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th odd squarefree semiprime s = p*q such that
   both p+q and |p−q| are not squarefree.
   Sequence is OEIS A364028.  Computation is done by a simple
   forward search with memoization. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/is_squarefree.h"
#include "../utils/mulmod.h"
#include "../utils/mem.h"

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
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

seq_t a(idx_t n)
{
    /* static memoization of sequence values */
    static seq_t *cache = NULL;          /* array of terms */
    static size_t cache_len = 0;         /* number of stored terms */
    static size_t cache_cap = 0;         /* allocated capacity */
    static uint64_t last_checked = 1;    /* largest integer inspected so far */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        uint64_t m = last_checked + 1;   /* next integer to test */

        /* Find the next valid odd semiprime */
        for (;;) {
            /* Skip even numbers */
            if ((m & 1ULL) == 0) { m++; continue; }

            /* Factor m; if m is prime or has no factor, skip */
            uint64_t p = smallest_prime_factor(m);
            if (p == 0 || p == m) { m++; continue; }

            uint64_t q = m / p;
            if (p == q || q == 0) { m++; continue; }   /* square of a prime */

            /* Ensure both factors are (odd) primes */
            if (!is_prime_big((seq_t)q)) { m++; continue; }

            /* Check squarefree conditions on sum and difference */
            seq_t sum = (seq_t)p + (seq_t)q;
            seq_t diff = (p > q) ?
                          (seq_t)(p - q) :
                          (seq_t)(q - p);
            if (is_squarefree(sum) || is_squarefree(diff)) { m++; continue; }

            /* Found a valid term */
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)m);
            break;
        }

        last_checked = m;
    }

    return cache[n - 1];
}
