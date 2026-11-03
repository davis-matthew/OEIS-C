/*
 * A345664  Primes whose digit reversal is the product of two (not necessarily
 *          distinct) emirps.
 * https://oeis.org/A345664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   factor_u64, is_prime, is_prime_u64, mem, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of primes p such that the decimal digit
   reversal rev(p) is a product of two emirps (primes whose
   reversal is a different prime).  The term a(n) is the n‑th such
   prime in increasing order.  The function generates successive
   primes and tests each one by factorising rev(p).  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* utility functions from the provided library */
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/reverse_digits_idx.h"
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

/* ---------- internal cache for the sequence ---------- */
static seq_t *seq_cache   = NULL;
static size_t  seq_cache_len = 0;
static size_t  seq_cache_cap = 0;

/* append an element to the cache, resizing on demand */
static void seq_cache_append(seq_t v)
{
    if (seq_cache_len == seq_cache_cap) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 64;
        seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }
    seq_cache[seq_cache_len++] = v;
}

/* ---------- helper to test whether a prime is an emirp ---------- */
static inline bool is_emirp(uint64_t x)
{
    if (x <= 1) return false;
    idx_t revx = reverse_digits_idx(x);          /* rev operates on idx_t = uint64_t */
    if (revx == x) return false;  /* palindromic primes are not emirps */
    /* x is prime by construction (from factorisation), only need to test rev(x) */
    return is_prime_big((seq_t)x) && is_prime_big((seq_t)revx);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* undefined for n==0; return 0 for safety */
    if (n == 0) return 0;

    /* generate terms until we have at least n of them */
    static uint64_t next_candidate = 2;   /* next number to test for primality */
    while (seq_cache_len < (size_t)n) {
        uint64_t p;

        /* find the next prime p */
        if (next_candidate == 2) {
            p = 2;
            next_candidate = 3;
        } else {
            /* skip even numbers */
            while (!is_prime(next_candidate))
                next_candidate += 2;
            p = next_candidate;
            next_candidate += 2;
        }

        /* candidate prime p: check its reversal */
        uint64_t revp = reverse_digits_idx(p);

        /* factorise rev(p) */
        uint64_t pr[32];
        uint64_t ex[32];
        size_t cnt = 0;
        factor_u64(revp, pr, ex, &cnt);

        bool ok = false;
        if (cnt == 1 && ex[0] == 2) {
            /* rev(p) is a square of a prime e*e */
            uint64_t e = pr[0];
            if (is_emirp(e)) {
                ok = true;
            }
        } else if (cnt == 2 && ex[0] == 1 && ex[1] == 1) {
            /* rev(p) is the product of two distinct primes e1*e2 */
            uint64_t e1 = pr[0];
            uint64_t e2 = pr[1];
            if (is_emirp(e1) && is_emirp(e2)) {
                ok = true;
            }
        }

        if (ok) {
            seq_cache_append((seq_t)p);
        }
    }

    return seq_cache[n - 1];
}
