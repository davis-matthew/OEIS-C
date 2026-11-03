/*
 * A347226  Safe primes (A005385) that are half-period primes (A097443).
 * https://oeis.org/A347226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u64, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search successive primes p, testing whether p is a safe prime
   ((p‑1)/2 is also prime) and a half‑period prime (the multiplicative order of
   10 modulo p is (p‑1)/2). The function is memoized so each index can be
   requested in any order. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/powmod.h"
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

static seq_t      *cache   = NULL;   /* array of found terms */
static size_t      cache_len  = 0;   /* number of stored terms */
static size_t      cache_cap  = 0;   /* current capacity of cache */
static uint64_t    next_pr   = 3;    /* next integer to test for primality */

/* extend the cache until it contains at least 'needed' terms */
static void extend_seq(idx_t needed)
{
    while (cache_len < (size_t)needed) {
        /* find next prime candidate */
        while (!is_prime_big((seq_t)next_pr)) {
            ++next_pr;
        }

        seq_t p  = (seq_t)next_pr;
        uint64_t sp = (next_pr - 1) / 2;          /* (p-1)/2  */
        if (!is_prime_big((seq_t)sp)) {           /* not a safe prime */
            ++next_pr;
            continue;
        }

        /* check half‑period property: 10^(sp) ≡ 1 (mod p) ? */
        if (powmod((seq_t)10, sp, p) != 1) {
            ++next_pr;
            continue;
        }

        /* compute the minimal order dividing sp */
        uint64_t ord = sp;
        uint64_t pf[64], pe[64];
        size_t pcnt;
        factor_u64(sp, pf, pe, &pcnt);
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t r = pf[i];
            while (ord % r == 0) {
                if (powmod((seq_t)10, ord / r, p) == 1) {
                    ord /= r;
                } else {
                    break;
                }
            }
        }

        if (ord == sp) {                         /* p is a half‑period prime */
            append_candidate(&cache, &cache_len, &cache_cap, p);
        }
        ++next_pr;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */
    if ((size_t)n > cache_len) extend_seq(n);
    return cache[(size_t)n-1];
}
