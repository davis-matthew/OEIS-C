/*
 * A285812  Primes equal to a centered 9-gonal number plus 1.
 * https://oeis.org/A285812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form (9*n*(n-1))/2 + 2
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 981 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of primes p such that p = C9(k)+1 where 
   centered 9‑gonal numbers are C9(k) = 9*k*(k-1)/2 + 1.
   Hence p = 9*k*(k-1)/2 + 2.  The function returns the n‑th such prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

static seq_t *prime_cache = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;
static idx_t   next_k     = 1;        /* next k to test for the form */

/* Ensure cache contains at least "need" primes. */
static void ensure_cache(idx_t need)
{
    while (cache_len < (size_t)need) {
        /* Compute p(k) = 9*k*(k-1)/2 + 2 as seq_t */
        seq_t k = (seq_t)next_k;
        seq_t p = (seq_t)9 * k * (k - 1) / 2 + 2;
        if (is_prime_big(p))            /* test primality of p */
            append_candidate(&prime_cache, &cache_len, &cache_cap, p);
        ++next_k;                       /* advance to next k */
    }
}

seq_t a(idx_t n)
/* Returns the n-th term of the sequence (1‑based). */
{
    if (n < 1) return 0;           /* undefined for n < 1 */
    ensure_cache(n);
    return prime_cache[n - 1];
}
