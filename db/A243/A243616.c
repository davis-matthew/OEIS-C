/*
 * A243616  Primes p of the form prime(n) + digitsum(n).
 * https://oeis.org/A243616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 627, u32 n <= 2535, u64 n <= 2535,
 *            u128 n <= 2535, big any n
 * Validated: standard, pass, 66 terms through n = 2534 (2026-11-03)
 * Helpers:   build_primes_upto, digit_sum, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243616: primes p such that p = prime(n) + digit_sum(n) */
/* a(1) = 3, a(2) = 5, a(3) = 11, a(4) = 19, a(5) = 53, … */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/digit_sum.h"
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

/* cached data ----------------------------------------------------------- */

static uint64_t *prime_list   = NULL;   /* primes up to a value limit */
static size_t   prime_count   = 0;      /* number of primes in prime_list */

static seq_t *seq_cache        = NULL;   /* sequence values a(i) */
static size_t seq_len          = 0;      /* number of sequence values cached */
static size_t seq_cap          = 0;      /* cache capacity */

static idx_t next_candidate    = 1;      /* next n to test for prime(n)+sum(n) */

/* --------------------------------------------------------------------- */

static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_cache = mem_resize(seq_t, seq_cache, new_cap);
    seq_cap   = new_cap;
}

/* Resize the prime table so that it contains at least idx primes. */
static void ensure_prime_table(idx_t idx)
{
    if (prime_list == NULL) {
        /* initial table – large enough for n up to at least 20 000 */
        const idx_t LIMIT = 250000;          /* value limit, not count */
        build_primes_upto(LIMIT, &prime_list, &prime_count);
    }
    if (idx <= prime_count) return;

    /* Need more primes – increase the value limit roughly twice the last prime. */
    idx_t new_limit = prime_list[prime_count - 1] * 2 + 100;
    free(prime_list);
    build_primes_upto(new_limit, &prime_list, &prime_count);
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for offset 1, harmless */

    /* Generate terms until we have the n‑th one */
    while (seq_len < (size_t)n) {
        /* Ensure we have enough primes to evaluate prime(next_candidate) */
        ensure_prime_table(next_candidate);

        /* Compute candidate p = prime(next_candidate) + digit_sum(next_candidate) */
        seq_t cand = (seq_t)prime_list[next_candidate - 1] + digit_sum((seq_t)next_candidate);

        if (is_prime_big(cand)) {
            ensure_seq_capacity(seq_len + 1);
            seq_cache[seq_len++] = cand;
        }

        ++next_candidate;   /* move to next n regardless of primality */
    }

    return seq_cache[(size_t)n - 1];
}
