/*
 * A261792  Primes of the form k*pi(k) - 1, where pi(k) is the number of
 *          primes <= k.
 * https://oeis.org/A261792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime of form k*pi(k) - 1, where pi(k) = number of primes
 *            <= k Algorithm: iterate k from 2 upward, maintain pi(k), compute
 *            v = k*pi(k)-1, test primality, and collect the primes until the
 *            requested number of terms is reached. Results are cached for
 *            subsequent calls
 * Range:     u8 n <= 11, u16 n <= 129, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9370 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
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

static seq_t *terms = NULL;          /* cached sequence values */
static size_t terms_size = 0;        /* number of values stored */
static size_t terms_cap  = 0;        /* current allocated capacity */

static idx_t next_k = 2;             /* next k to be processed (starting at 2) */
static idx_t pi     = 0;             /* pi(k-1) – number of primes <= next_k-1 */

/* Grow the terms array to at least min_cap elements */
static void grow_terms(size_t min_cap)
{
    if (terms_cap >= min_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, terms, new_cap);
    terms = tmp;
    terms_cap = new_cap;
}

/* Ensure that at least 'needed' terms are computed and cached */
static void ensure_terms(size_t needed)
{
    if (terms_size >= needed) return;

    for (; terms_size < needed; ++next_k)
    {
        /* Update pi(k) */
        if (is_prime_seq((seq_t)next_k))
            ++pi;

        /* Compute candidate value v = k*pi(k) - 1 */
        seq_t val = (seq_t)next_k * (seq_t)pi - (seq_t)1;

        /* Check if v is prime and store it if so */
        if (is_prime_big(val))
        {
            grow_terms(terms_size + 1);
            terms[terms_size++] = val;
        }
    }
}

/* Public function required by the benchmark framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* defensive; not used (offset is 1) */
    ensure_terms((size_t)n);
    return terms[n-1];
}
