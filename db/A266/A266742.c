/*
 * A266742  Irregular triangle read by rows: T(n,k) = floor(n/(prime(k)-1)),
 *          n>=1, 1 <= k <= pi(n+1), where pi is A000720.
 * https://oeis.org/A266742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = floor(r/(prime(k)-1)), where r and k are defined by *
 *            cumulative = Σ_{i=1}^{r} π(i+1) ≥ n * prev_cumulative =
 *            cumulative - π(r+1) * k = n - prev_cumulative , 1 ≤ k ≤ π(r+1) *
 *            i.e. a(n) is the n‑th term in the irregular triangle read by
 *            rows
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- static prime cache ---------- */
static uint64_t * _primes = NULL;      /* array of primes in ascending order   */
static size_t    _primes_cnt = 0;      /* number of primes stored              */
static uint64_t  _primes_limit = 0;    /* largest number up to which primes were generated */

/* generate all primes ≤ limit (simple sieve) and store them in _primes */
static void _generate_primes_upto(uint64_t limit)
{
    if (limit < 2) {
        _primes_cnt = 0;
        free(_primes);
        _primes = NULL;
        _primes_limit = 0;
        return;
    }

    /* allocate composite marker array */
    uint8_t *comp = (uint8_t*)malloc((size_t)(limit + 1));
    if (!comp) return; /* allocation failed – keep old data or empty state */

    /* initialize */
    for (size_t i = 0; i <= limit; i++) comp[i] = 0;

    /* sieve */
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!comp[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                comp[j] = 1;
        }
    }

    /* count primes to allocate array */
    size_t count = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!comp[i]) ++count;

    uint64_t *new_primes = (uint64_t*)realloc(_primes, count * sizeof(uint64_t));
    if (!new_primes) {
        free(comp);
        return; /* ignore failure – old array keeps being used */
    }

    _primes = new_primes;
    _primes_cnt = count;

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!comp[i]) _primes[idx++] = i;

    free(comp);
    _primes_limit = limit;
}

/* ensure that all primes up to 'limit' are generated */
static void _ensure_primes_up_to(uint64_t limit)
{
    if (limit > _primes_limit)
        _generate_primes_upto(limit);
}

/* return π(x): the number of primes ≤ x (x is uint64_t) */
static size_t _prime_upto(uint64_t x)
{
    if (x < 2) return 0;
    size_t lo = 0, hi = _primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (_primes[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1, but return 0 safely */

    /* Find the row r such that
     *   cumulative = Σ_{i=1}^{r} π(i+1)  ≥  n
     *   and cumulative - π(r+1) < n
     */
    idx_t r = 0;
    idx_t cumulative = 0;
    idx_t prev_cumulative = 0;

    while ((idx_t)cumulative < n) {
        ++r;
        /* need primes up to r+1 for counting */
        _ensure_primes_up_to((uint64_t)r + 1);
        size_t pi_r = _prime_upto((uint64_t)r + 1);
        prev_cumulative = cumulative;
        cumulative = (idx_t)(cumulative + (idx_t)pi_r);
    }

    /* Position k within the current row (1‐based) */
    idx_t k = n - prev_cumulative;          /* 1 ≤ k ≤ π(r+1) */

    /* index k-1 because array zero based */
    uint64_t prime_k = _primes[(size_t)(k - 1)];

    /* Compute T(r,k) = floor(r / (prime(k)-1)) */
    seq_t denom = (seq_t)(prime_k - 1);      /* denom ≥ 1 */
    seq_t rn    = (seq_t)r;
    return rn / denom;
}
