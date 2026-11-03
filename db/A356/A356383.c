/*
 * A356383  Primes p such that the sum of p and the next four primes congruent
 *          mod 10 to p is 5 times a prime.
 * https://oeis.org/A356383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 767, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9370 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find primes p such that p + the next four primes congruent to p (mod 10)
   equals 5 times a prime.  The function builds the sequence on demand,
   memoizing the terms so repeated calls are fast. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
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

static seq_t *terms = NULL;          /* memoized sequence terms */
static size_t terms_cap = 0;          /* capacity of the terms array */
static size_t terms_len = 0;          /* number of stored terms */
static uint64_t nextprime_start = 0; /* start for the next prime search */

/* Return the n‑th term (1‑based) of A356383 */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety; real offset is 1 */

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* Next prime after the previous one */
        uint64_t p = next_prime(nextprime_start);
        nextprime_start = p;

        /* Small primes < 7 cannot have four distinct successors with the
           required residue, so skip them */
        if (p < 7) continue;

        uint64_t q = p;         /* current prime candidate */
        uint8_t target_mod = (uint8_t)(p % 10);
        seq_t sum = (seq_t)p;   /* start the sum with p itself */

        /* Find the next four primes congruent to p (mod 10) and add them
           to the sum.  next_prime always returns the next prime > start,
           so we can loop until the modulo matches. */
        for (int i = 0; i < 4; ++i) {
            do {
                q = next_prime(q);
            } while ((q % 10) != target_mod);
            sum += (seq_t)q;
        }

        /* sum is always divisible by 5; check whether sum/5 is prime. */
        seq_t mid = sum / 5; /* division by an integral constant */
        if (!is_prime_big(mid)) continue; /* not a term */

        /* Ensure enough space in the memoization array */
        if (terms_len == terms_cap) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 64;
            seq_t *tmp = mem_resize(seq_t, terms, new_cap);
            terms = tmp;
            terms_cap = new_cap;
        }
        terms[terms_len++] = (seq_t)p;
    }

    return terms[n - 1];
}
