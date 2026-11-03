/*
 * A280084  1 together with the Pythagorean primes.
 * https://oeis.org/A280084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 1 if n = 1, otherwise the (n‑1)‑th prime congruent to 1
 *            mod 4
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a cache of primes of the form 4k+1.
   It grows the cache on demand and uses the utilities
   seq_realloc (for arbitrary precision) and is_prime_big
   (principal primality test). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
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

static seq_t *p_list     = NULL;   /* array holding the primes of the form 4k+1 */
static size_t p_count    = 0;      /* number of primes stored so far */
static size_t p_capacity = 0;      /* current capacity of p_list */
static uint64_t next_candidate = 5;/* next candidate ≡ 1 (mod 4) */

/* ------------------------------------------------------------------
 * Return a(idx_t n): nth term of sequence A280084.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;                 /* a(1) = 1 */

    /* Number of 4k+1 primes that must be found. */
    const idx_t need = n - 1;            /* 1‑based count of required primes */

    /* Grow the cache until it contains at least 'need' primes. */
    while (p_count < (size_t)need) {
        /* Grow storage if necessary. */
        if (p_count == p_capacity) {
            const size_t new_cap = p_capacity ? p_capacity * 2 : 16;
            seq_t *tmp = seq_realloc(p_list, p_capacity, new_cap);
            if (!tmp)
                abort();                  /* allocation failed */
            p_list     = tmp;
            p_capacity = new_cap;
        }

        /* Find the next 4k+1 prime. */
        for (;;) {
            const uint64_t cand = next_candidate;
            next_candidate += 4;          /* next odd number ≡ 1 (mod 4) */

            const seq_t cand_seq = (seq_t)cand;
            if (is_prime_big(cand_seq)) {
                p_list[p_count++] = cand_seq;
                break;
            }
        }
    }

    /* Return the (need‑1)‑th element (zero‑based index). */
    return p_list[(size_t)need - 1];
}
