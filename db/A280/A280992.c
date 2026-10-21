/*
 * A280992  Squarefree triangular numbers that are products of consecutive
 *          primes.
 * https://oeis.org/A280992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280992: square‑free triangular numbers that are products of consecutive primes.
   Computed by scanning triangular indices up to MAX_K. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_K      5000000U   /* maximal triangular index that will be checked */
#define CACHE_CAP  256        /* capacity of the cache of found terms */

static bool          _initialized = false;
static uint32_t     * _spf        = NULL;
static uint32_t     * _prime_idx  = NULL;

static seq_t        _cache[CACHE_CAP];
static size_t        _cache_len   = 0;

static void _init(void)
{
    if (_initialized) return;
    _initialized = true;

    uint32_t limit = MAX_K + 1U;
    _spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!_spf) abort();
    for (uint32_t i = 0; i <= limit; ++i) _spf[i] = i;
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (_spf[i] == i) {
            for (uint32_t j = i * i; j <= limit; j += i)
                if (_spf[j] == j) _spf[j] = i;
        }
    }

    _prime_idx = (uint32_t*)malloc((limit + 2) * sizeof(uint32_t));
    if (!_prime_idx) abort();
    for (uint32_t i = 0; i <= limit + 1; ++i) _prime_idx[i] = 0xFFFFFFFFU;
    uint32_t pos = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (_spf[i] == i) _prime_idx[i] = pos++;

    _cache_len = 0;
}

static void _precompute(void)
{
    if (_cache_len != 0) return;   /* already computed */

    uint32_t k;
    for (k = 1; k <= MAX_K; ++k) {
        /* factor k */
        uint32_t pk[32], ek[32];
        uint32_t ck = 0;
        uint32_t n = k;
        while (n > 1) {
            uint32_t p = _spf[n];
            uint32_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            pk[ck] = p; ek[ck] = e; ++ck;
        }

        /* factor k+1 */
        uint32_t pk1[32], ek1[32];
        uint32_t ck1 = 0;
        n = k + 1U;
        while (n > 1) {
            uint32_t p = _spf[n];
            uint32_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            pk1[ck1] = p; ek1[ck1] = e; ++ck1;
        }

        /* merge the two factorizations */
        uint32_t pf[32], ef[32];
        uint32_t cf = 0;
        uint32_t i = 0, j = 0;
        while (i < ck && j < ck1) {
            if (pk[i] < pk1[j]) {
                pf[cf] = pk[i]; ef[cf] = ek[i]; ++i;
            } else if (pk[i] > pk1[j]) {
                pf[cf] = pk1[j]; ef[cf] = ek1[j]; ++j;
            } else {
                pf[cf] = pk[i]; ef[cf] = ek[i] + ek1[j]; ++i; ++j;
            }
            ++cf;
        }
        while (i < ck) { pf[cf] = pk[i]; ef[cf] = ek[i]; ++i; ++cf; }
        while (j < ck1) { pf[cf] = pk1[j]; ef[cf] = ek1[j]; ++j; ++cf; }

        /* subtract the factor 2 coming from the division by 2 in T_k = k*(k+1)/2 */
        uint32_t pos2 = 0xFFFFFFFFU;
        for (i = 0; i < cf; ++i) {
            if (pf[i] == 2U) { pos2 = i; break; }
        }
        if (pos2 == 0xFFFFFFFFU) continue;   /* should not happen */

        if (--ef[pos2] == 0) {
            for (i = pos2; i + 1 < cf; ++i) {
                pf[i] = pf[i + 1];
                ef[i] = ef[i + 1];
            }
            --cf;
        }

        /* check square‑free : every exponent must be 1 */
        bool squarefree = true;
        for (i = 0; i < cf; ++i) {
            if (ef[i] != 1U) { squarefree = false; break; }
        }
        if (!squarefree) continue;

        /* check that the primes are consecutive.  Empty product (cf==0) is allowed. */
        if (cf != 0) {
            uint32_t start_idx = _prime_idx[pf[0]];
            if (start_idx == 0xFFFFFFFFU) continue;
            for (i = 0; i < cf; ++i) {
                if (_prime_idx[pf[i]] != start_idx + i) { squarefree = false; break; }
            }
            if (!squarefree) continue;
        }

        /* success: store the triangular number */
        if (_cache_len < CACHE_CAP) {
            _cache[_cache_len++] = (seq_t)k * ((seq_t)k + (seq_t)1) / (seq_t)2;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 */

    _init();
    _precompute();

    if ((size_t)n <= _cache_len) return _cache[n-1];
    return (seq_t)0;                       /* term not found within searched range */
}
