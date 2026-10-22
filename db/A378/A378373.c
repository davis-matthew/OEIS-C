/*
 * A378373  Number of composite numbers (A002808) between consecutive
 *          nonsquarefree numbers (A013929), exclusive.
 * https://oeis.org/A378373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of composite integers strictly between the
 *            (n–1)st and nth nonsquarefree numbers (A013929)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static idx_t   _a_sieve_limit   = 0;            /* current maximal number sieved */
static uint8_t *_a_is_prime     = NULL;        /* prime flag array */
static uint8_t *_a_is_nonsq     = NULL;        /* nonsquarefree flag array */
static uint32_t *_a_prime_pref  = NULL;        /* prefix count of primes */
static idx_t   *_a_nonsq_list   = NULL;        /* list of nonsquarefree numbers */
static size_t   _a_nonsq_count  = 0;            /* number of elements stored */

/* re‑build the sieve up to a new upper bound -------------------------------- */
static void _a_extend_sieve(idx_t new_limit)
{
    if (new_limit <= _a_sieve_limit)
        return;

    /* Free previous data. */
    free(_a_is_prime);
    free(_a_is_nonsq);
    free(_a_prime_pref);
    free(_a_nonsq_list);

    /* Allocate flag arrays. */
    _a_is_prime = (uint8_t*)calloc((size_t)new_limit + 1, sizeof(uint8_t));
    _a_is_nonsq = (uint8_t*)calloc((size_t)new_limit + 1, sizeof(uint8_t));
    if (!_a_is_prime || !_a_is_nonsq)
        abort();

    /* Sieve for primes. */
    for (idx_t i = 2; i <= new_limit; ++i)
        _a_is_prime[i] = 1;                      /* mark as prime initially */
    for (idx_t i = 2; i * i <= new_limit; ++i)
        if (_a_is_prime[i])
            for (idx_t j = i * i; j <= new_limit; j += i)
                _a_is_prime[j] = 0;

    /* Mark nonsquarefree numbers: multiples of p^2. */
    for (idx_t p = 2; p * p <= new_limit; ++p)
        if (_a_is_prime[p]) {
            idx_t p2 = p * p;
            for (idx_t j = p2; j <= new_limit; j += p2)
                _a_is_nonsq[j] = 1;
        }

    /* Build prime prefix array. */
    _a_prime_pref = (uint32_t*)malloc(((size_t)new_limit + 1u) * sizeof(uint32_t));
    if (!_a_prime_pref)
        abort();
    _a_prime_pref[0] = 0;
    for (idx_t i = 1; i <= new_limit; ++i)
        _a_prime_pref[i] = _a_prime_pref[i - 1] + (_a_is_prime[i] ? 1 : 0);

    /* Collect nonsquarefree numbers. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (_a_is_nonsq[i]) ++cnt;
    _a_nonsq_list = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!_a_nonsq_list)
        abort();
    size_t idx = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (_a_is_nonsq[i]) _a_nonsq_list[idx++] = i;

    _a_nonsq_count = cnt;
    _a_sieve_limit = new_limit;
}

/* ensure that we have at least (needed) nonsquarefree numbers in the cache */
static void _a_ensure_nonsq(size_t needed)
{
    if (_a_nonsq_count >= needed)
        return;
    idx_t lim = _a_sieve_limit ? _a_sieve_limit * 2 : 200;
    while (_a_nonsq_count < needed) {
        if (lim == 0)
            lim = 200;
        else
            lim *= 2;
        _a_extend_sieve(lim);
    }
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n >= 1 according to OEIS offset. */
    _a_ensure_nonsq((size_t)n + 1);

    idx_t low  = _a_nonsq_list[n - 1];
    idx_t high = _a_nonsq_list[n];

    /* Count integers strictly between low and high. */
    idx_t total = high - low - 1;                         /* numbers in the open interval */

    /* Count primes inside that interval. */
    idx_t primes_in = _a_prime_pref[high - 1] - _a_prime_pref[low];

    /* Composite count = total – primes_in (zero is not counted). */
    seq_t res = (seq_t)(total - primes_in);
    return res;
}
