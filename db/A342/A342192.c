/*
 * A342192  Heinz numbers of partitions of crank 0.
 * https://oeis.org/A342192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Heinz number of the n-th partition whose crank is 0
 *            (i.e., the number of parts > number_of_ones equals
 *            number_of_ones and there is at least one part equal to 1)
 * Range:     u8 n <= 34, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function enumerates integers, maps each to its unique partition
   via prime‑indices, tests the crank condition, and memoizes the first
   200 000 terms that satisfy it.  Derivations use a dynamic small prime
   factor sieve.  It works for any seq_t/idx_t supplied by the build
   system. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- Local sieve structure ---------- */
typedef struct {
    size_t size;          /* current maximum value covered (inclusive) */
    uint32_t *spf;        /* smallest prime factor for each value */
    uint32_t *prime_idx;  /* 1‑based index of each prime, 0 otherwise */
} _A342192_sieve_t;

static _A342192_sieve_t _A342192_sieve = {0, NULL, NULL};

/* Build/extend the sieve up to at least 'limit' (inclusive). */
static void _A342192_build_sieve_to(size_t limit)
{
    if (limit <= _A342192_sieve.size) return;          /* already built */

    size_t n = limit + 1;
    uint32_t *spf = (uint32_t *)calloc(n, sizeof(uint32_t));
    if (!spf) exit(1);  /* memory error */
    for (size_t i = 2; i < n; ++i) spf[i] = (uint32_t)i;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {          /* i is prime */
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }

    uint32_t *prime_idx = (uint32_t *)calloc(n, sizeof(uint32_t));
    if (!prime_idx) { free(spf); exit(1); }

    uint32_t cnt = 0;
    for (size_t i = 2; i < n; ++i) {
        if (spf[i] == (uint32_t)i) {          /* i is prime */
            ++cnt;
            prime_idx[i] = cnt;              /* 1‑based index */
        }
    }

    free(_A342192_sieve.spf);
    free(_A342192_sieve.prime_idx);
    _A342192_sieve.spf = spf;
    _A342192_sieve.prime_idx = prime_idx;
    _A342192_sieve.size = limit;
}

/* Test whether the integer 'val' maps to a partition with crank 0. */
static bool _A342192_is_crank0(seq_t val)
{
    if (val < 2) return false;                /* 0 and 1 are not partition numbers */

    uint64_t n = (uint64_t)val;               /* value is assumed < 2^64 */
    _A342192_build_sieve_to((size_t)n);       /* ensure sieve covers n */

    int ones = 0;                             /* number of 1‑parts */
    int parts[64];                            /* indices of parts > 1 */
    int pc = 0;                               /* count of parts > 1 */

    uint64_t tmp = n;
    while (tmp > 1) {
        uint32_t p = _A342192_sieve.spf[tmp];
        uint32_t idx = _A342192_sieve.prime_idx[p];
        if (idx == 1) {
            ++ones;
        } else {
            if (pc < 64) parts[pc++] = idx;
        }
        tmp /= p;
    }

    if (ones == 0) return false;              /* need at least one 1 */

    int gt = 0;                               /* parts > ones */
    for (int i = 0; i < pc; ++i)
        if (parts[i] > ones) ++gt;

    return (gt == ones);
}

/* ---------- Cached sequence generation ---------- */
#define A342192_CACHE_CAP 200000U

static seq_t _A342192_cache[A342192_CACHE_CAP];
static size_t _A342192_cache_size = 0;
static seq_t _A342192_next_candidate = 2;

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety: OEIS offset is 1 */

    /* Generate terms until we have at least 'n' values */
    while (_A342192_cache_size < (size_t)n) {
        if (_A342192_is_crank0(_A342192_next_candidate)) {
            if (_A342192_cache_size < A342192_CACHE_CAP)
                _A342192_cache[_A342192_cache_size++] = _A342192_next_candidate;
        }
        ++_A342192_next_candidate;
    }

    return _A342192_cache[(size_t)n - 1];
}
