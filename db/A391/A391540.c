/*
 * A391540  Terms of A055932 that are products of the three composite integers
 *          in prime gaps of size 4.
 * https://oeis.org/A391540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391540: Terms of A055932 that are products of the three composite integers in prime gaps of size 4.
   The code computes all prime gaps p,p+4 (p and p+4 primes, and p+1,p+2,p+3 composite),
   then tests whether the product (p+1)(p+2)(p+3) is "prime‑complete"
   (contains every prime up to its largest prime divisor).
   The known sequence has 6 terms; the algorithm stops after collecting
   all such gaps within a small bound (2000) that suffices for those terms. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Small prime sieve – we only need primes up to 2000 for the known
   6 terms (the largest term involves p = 439). */
#define PRIMES_MAX 2000
static uint8_t _prime_flag[PRIMES_MAX + 1];
static uint32_t _prime_list[PRIMES_MAX];
static size_t _prime_cnt = 0;

static void _init_primes(void)
{
    size_t i, j;
    for (i = 0; i <= PRIMES_MAX; ++i) _prime_flag[i] = 1;
    _prime_flag[0] = _prime_flag[1] = 0;
    for (i = 2; i * i <= PRIMES_MAX; ++i) {
        if (_prime_flag[i]) {
            for (j = i * i; j <= PRIMES_MAX; j += i)
                _prime_flag[j] = 0;
        }
    }
    for (i = 2; i <= PRIMES_MAX; ++i)
        if (_prime_flag[i]) _prime_list[_prime_cnt++] = (uint32_t)i;
}

/* ------------------------------------------------------------------ */
/* Sequence cache – holds all terms that have been found.  It is built
   lazily on the first call to a(). */
static seq_t *_term_cache = NULL;
static size_t _term_cnt   = 0;
static bool _initialized = false;

static void _compute_terms(void)
{
    /* Allocate string large enough to hold all possible terms within
       the bound (worst case 2000 primes → far fewer than 10 terms). */
    _term_cache = (seq_t *)malloc(16 * sizeof(seq_t));
    if (!_term_cache) exit(1);
    _term_cnt = 0;

    for (size_t idx = 0; idx + 4 <= _prime_cnt; ++idx) {
        uint32_t p = _prime_list[idx];
        if (p + 4 > PRIMES_MAX) break;
        if (!_prime_flag[p + 4]) continue;                 /* gap size 4 but not both primes */
        if (_prime_flag[p + 1] || _prime_flag[p + 2] || _prime_flag[p + 3])
            continue;                                       /* not all composites */

        /* m = p+1  (start of the 3 composites) */
        uint32_t m = p + 1;
        uint64_t prod64 = (uint64_t)m * (m + 1) * (m + 2);
        seq_t prod = (seq_t)prod64;                         /* fits in any seq_t */

        /* Test prime‑complete: must contain every prime <= its max prime factor */
        uint32_t max_prime = 0;
        bool ok = true;
        for (size_t pi = 0; pi < _prime_cnt && _prime_list[pi] <= m + 2; ++pi) {
            uint32_t q = _prime_list[pi];
            if (prod64 % q == 0) {
                if (q > max_prime) max_prime = q;
            } else {
                /* if we already know a larger prime appears in the product,
                   missing this one makes the set incomplete */
                if (q <= max_prime) { ok = false; break; }
            }
        }
        if (ok && max_prime > 0) {
            /* Verify that all primes <= max_prime indeed divide prod */
            for (size_t pi = 0; pi < _prime_cnt && _prime_list[pi] <= max_prime; ++pi) {
                if (prod64 % _prime_list[pi] != 0) { ok = false; break; }
            }
        }
        if (ok) {
            _term_cache[_term_cnt++] = prod;
        }
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!_initialized) {
        _init_primes();
        _compute_terms();
        _initialized = true;
    }
    if (n == 0) return (seq_t)0;                     /* offset 1 – behaviour undefined below */
    if ((idx_t)n > (idx_t)_term_cnt) return (seq_t)0; /* beyond the finite sequence */
    return _term_cache[n - 1];
}
