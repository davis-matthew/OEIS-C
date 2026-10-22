/*
 * A302798  Squarefree numbers that are prime or whose prime indices are
 *          pairwise coprime. Heinz numbers of strict integer partitions that
 *          either consist of a single part or have pairwise coprime parts.
 * https://oeis.org/A302798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   gcd_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A302798 – squarefree numbers whose prime indices are pairwise coprime.
   The function generates integers in ascending order, storing them in a cache
   so that subsequent calls for any n are answered in O(1) after the initial
   sieve. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd_u32.h"

static bool init_done = false;

/* Cached sequence values */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static idx_t  next_candidate = 1;

/* Sieve data */
static uint64_t *spf          = NULL;   /* smallest prime factor */
static uint32_t *prime_index  = NULL;   /* index of a prime (1‑based) */
static uint64_t  sieve_limit  = 0;       /* current sieve upper bound */

/* GCD for unsigned 32‑bit integers */

/* Allocate or grow the sequence cache */
static void
grow_seq_cache(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap > 0 ? seq_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Perform a linear sieve up to `limit`, storing spf and prime indices. */
static void
sieve(uint64_t limit)
{
    if (limit <= sieve_limit)
        return;
    /* reallocate to the larger limit */
    spf       = (uint64_t *)realloc(spf,      (limit + 1) * sizeof(uint64_t));
    prime_index = (uint32_t *)realloc(prime_index, (limit + 1) * sizeof(uint32_t));
    memset(spf, 0, (limit + 1) * sizeof(uint64_t));
    memset(prime_index, 0, (limit + 1) * sizeof(uint32_t));

    uint32_t prime_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                    /* i is prime */
            spf[i] = i;
            prime_cnt++;
            prime_index[i] = prime_cnt;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == 0)
                        spf[j] = i;
                }
            }
        }
    }
    sieve_limit = limit;
}

/* Test whether x is a square‑free number whose prime indices are pairwise coprime. */
static bool
is_good(uint64_t x)
{
    if (x == 1)
        return true;

    uint32_t idx_vec[16];
    size_t idx_cnt = 0;
    uint64_t t = x;

    while (t > 1) {
        uint64_t p = spf[t];
        if (p == 0)            /* should not happen for t <= sieve_limit */
            p = t;
        /* check for repeated prime factor (square) */
        if ((t / p) % p == 0)
            return false;
        uint32_t idx = prime_index[(uint32_t)p];
        for (size_t i = 0; i < idx_cnt; ++i)
            if (gcd_u32(idx_vec[i], idx) != 1)
                return false;
        idx_vec[idx_cnt++] = idx;
        t /= p;
    }
    return true;
}

/* One‑time initialization of sieve and cache. */
static void
init(void)
{
    if (init_done)
        return;
    init_done = true;

    /* Cache for the sequence, reserved size initially */
    seq_cap = 64;
    seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_len  = 0;

    /* Setup sieve with an upper bound expected to cover all required terms. */
    sieve_limit = 3000000;          /* 3 million should cover n ≤ 10000 */
    spf        = (uint64_t *)calloc(sieve_limit + 1, sizeof(uint64_t));
    prime_index = (uint32_t *)calloc(sieve_limit + 1, sizeof(uint32_t));

    uint32_t prime_cnt = 0;
    for (uint64_t i = 2; i <= sieve_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            prime_cnt++;
            prime_index[i] = prime_cnt;
            if (i * i <= sieve_limit) {
                for (uint64_t j = i * i; j <= sieve_limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }
}

/* Main sequence function requested by the build system. */
seq_t
a(idx_t n)
{
    init();
    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        uint64_t x = next_candidate;
        ++next_candidate;

        if (is_good(x)) {
            grow_seq_cache(seq_len + 1);
            seq_cache[seq_len] = (seq_t)x;
            ++seq_len;
        }
    }
    return seq_cache[n - 1];
}
