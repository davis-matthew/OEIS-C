/*
 * A309305  Sums of two primes whose difference is a nonzero square.
 * https://oeis.org/A309305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth element of OEIS A309305:
   a(n) = the nth integer that can be written as p + q,
   where p and q are primes and q - p = d^2 for some
   non‑zero integer d. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

static seq_t *terms = NULL;      /* cached sequence values   */
static size_t terms_len = 0;     /* number of cached terms   */
static size_t terms_cap = 0;     /* allocated capacity       */
static seq_t next_candidate = (seq_t)5; /* next integer to test */

/* Ensure that the cache contains at least n terms. */
static void
ensure_terms(idx_t n)
{
    if (terms == NULL) {
        terms_cap = 16;
        terms = mem_new(seq_t, terms_cap);
        if (terms == NULL) abort();
    }

    while (terms_len < n) {
        bool found = false;

        /* Search for the next admissible sum. */
        for (;;) {
            /* Iterate over squares d² with d ≥ 1 and d² < next_candidate. */
            for (seq_t d = (seq_t)1;; ++d) {
                seq_t d2 = d * d;
                if (d2 >= next_candidate) break;

                /* next_candidate - d² must be even for integer p. */
                if (((next_candidate - d2) & (seq_t)1) == (seq_t)0) {
                    seq_t p = (next_candidate - d2) / (seq_t)2;
                    seq_t q = p + d2;
                    if (is_prime_big(p) && is_prime_big(q)) {
                        found = true;
                        break;
                    }
                }
            }

            if (found) {
                /* Grow the cache if necessary. */
                if (terms_len == terms_cap) {
                    terms_cap <<= 1;
                    seq_t *tmp = mem_resize(seq_t, terms, terms_cap);
                    if (tmp == NULL) abort();
                    terms = tmp;
                }
                terms[terms_len++] = next_candidate;
                next_candidate++;
                break;  /* finish this candidate */
            }
            next_candidate++;  /* try the next integer */
        }
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) abort();
    ensure_terms(n);
    return terms[n - (idx_t)1];
}
