/*
 * A386158  Primes having only {2, 4, 7, 9} as digits.
 * https://oeis.org/A386158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9512 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A386158: primes having only digits {2,4,7,9} (search‑based implementation) */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

static seq_t *seq_cache = NULL;
static size_t seq_cache_len = 0;
static size_t seq_cache_cap = 0;

/* Allowed digits in decimal representation. */
static const uint8_t dmap[4] = { 2, 4, 7, 9 };

/* Current candidate expressed as indices into dmap.
   cand_inds[0] is the most significant digit. */
static uint8_t cand_inds[64];
static size_t cand_len = 0;

/* Initialise the candidate stack to the first prime candidate 2. */
static void init_candidate(void)
{
    if (seq_cache_len == 0) {
        cand_len = 1;
        cand_inds[0] = 0;            /* digit 2 */
    }
}

/* Convert the current digit indices to the numeric value. */
static seq_t cand_value(void)
{
    seq_t v = 0;
    for (size_t i = 0; i < cand_len; ++i)
        v = v * 10 + (seq_t)dmap[cand_inds[i]];
    return v;
}

/* Increment the candidate to the next number using only digits {2,4,7,9}. */
static void inc_candidate(void)
{
    int pos = (int)cand_len - 1;
    while (pos >= 0) {
        if (cand_inds[pos] == 3) {   /* maximum digit reached – wrap */
            cand_inds[pos] = 0;
            --pos;
        } else {
            ++cand_inds[pos];
            break;
        }
    }
    if (pos < 0) {   /* all digits rolled over – increase length */
        for (int i = (int)cand_len; i > 0; --i)
            cand_inds[i] = cand_inds[i - 1];
        cand_inds[0] = 0;
        ++cand_len;
    }
}

/* Extend the cache until it contains at least n primes. */
static void extend_cache(idx_t n)
{
    init_candidate();
    while (seq_cache_len < (size_t)n) {
        seq_t val = cand_value();
        if (is_prime_big(val))
            append_candidate(&seq_cache, &seq_cache_len, &seq_cache_cap, val);
        inc_candidate();
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined below offset */
    if ((size_t)n > seq_cache_len) extend_cache(n);
    return seq_cache[n - 1];
}
