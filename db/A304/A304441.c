/*
 * A304441  Numbers k such that 8k, 8k+1 and 8k+2 are the sum of two squares;
 *          A082982 / 8.
 * https://oeis.org/A304441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer k such that 8k, 8k+1 and 8k+2 are sums of two squares. */
/* Uses a small prime sieve for the sum‑of‑two‑squares test. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --- Prime sieve ------------------------------------------------------ */
static const unsigned _PRIME_SIEVE_LIMIT = 200000; /* ample for 10 000 terms */
static bool _primes_initialized = false;
static uint32_t *_primes = NULL;
static size_t _primes_cnt = 0;

static void _init_primes(void)
{
    if (_primes_initialized) return;

    bool *is_comp = (bool*)calloc(_PRIME_SIEVE_LIMIT + 1, sizeof(bool));
    for (size_t i = 2; i * i <= _PRIME_SIEVE_LIMIT; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= _PRIME_SIEVE_LIMIT; j += i)
                is_comp[j] = true;

    for (size_t i = 2; i <= _PRIME_SIEVE_LIMIT; ++i)
        if (!is_comp[i]) ++_primes_cnt;

    _primes = (uint32_t*)malloc(_primes_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= _PRIME_SIEVE_LIMIT; ++i)
        if (!is_comp[i]) _primes[idx++] = (uint32_t)i;

    free(is_comp);
    _primes_initialized = true;
}

/* --- Sum‑of‑two‑squares test -------------------------------------------- */
static bool _is_sum_of_two_squares(uint64_t n)
{
    if (n == 0 || n == 1) return true;
    _init_primes();

    uint64_t m = n;
    for (size_t i = 0; i < _primes_cnt; ++i) {
        uint32_t p = _primes[i];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > m) break;
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if ((p & 3) == 3 && (exp & 1)) return false;
    }
    if (m > 1 && (m & 3) == 3) return false;
    return true;
}

/* --- Sequence cache ----------------------------------------------------- */
static seq_t *_cache = NULL;
static size_t _cache_len = 0;
static size_t _cache_cap = 0;
static uint64_t _next_candidate = 0;

static void _ensure_cache_cap(size_t needed)
{
    if (_cache_cap >= needed) return;
    size_t new_cap = _cache_cap ? (_cache_cap * 2) : 128;
    while (new_cap < needed) new_cap *= 2;

    seq_t *tmp = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
    _cache = tmp;
    _cache_cap = new_cap;
}

/* --- Main function ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */

    size_t target = (size_t)n;
    while (_cache_len < target) {
        uint64_t k = _next_candidate;
        uint64_t v8 = k * 8ULL;

        if (_is_sum_of_two_squares(v8) &&
            _is_sum_of_two_squares(v8 + 1) &&
            _is_sum_of_two_squares(v8 + 2)) {
            _ensure_cache_cap(_cache_len + 1);
            _cache[_cache_len++] = (seq_t)k;
        }
        ++_next_candidate;
    }
    return _cache[target - 1];
}
