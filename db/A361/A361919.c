/*
 * A361919  The number of primes > A000040(n) and <= (A000040(n)^c + 1)^(1/c),
 *          where c = 0.567148130202... is defined in A038458.
 * https://oeis.org/A361919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 665, u16 n <= 665, u32 n <= 665, u64 n <= 665,
 *            u128 n <= 665, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A361919:
   a(n) = #{ primes p with A000040(n) < p ≤ ⌊(A000040(n)^c + 1)^(1/c)⌋ },
   where c ≈ 0.567148130202… is the unique real solution of
   127^c − 113^c = 1.  The function uses integer arithmetic for all
   seq_t operations and only long‑double arithmetic to compute the
   upper bound. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ------------------------------------------------------------ */
/* Helper to build a prime table up to a fixed limit.  The limit
   must be large enough to cover A000040(n) for n up to about 10 000.
   The 10 000‑th prime is 104 729, and (104 729^c + 1)^(1/c) ≈ 10⁵,
   so 200 000 is safe. */
#define PRIME_LIMIT 200000

static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static bool primes_ready = false;

/* Allocate and fill prime_list with all primes ≤ PRIME_LIMIT. */
static void init_primes(void)
{
    if (primes_ready) return;

    bool *is_prime = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime) { /* allocation failed; terminate */
        abort();
    }

    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        is_prime[i] = true;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;

    /* Count primes first to allocate correctly */
    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    prime_list = (uint32_t *)calloc(cnt, sizeof(uint32_t));
    if (!prime_list) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint32_t)i;

    prime_count = cnt;
    primes_ready = true;
    free(is_prime);
}

/* Binary search: last index with prime_list[idx] ≤ value.
   Returns SIZE_MAX if no such prime exists. */
static size_t last_le_idx(uint32_t value)
{
    size_t lo = 0, hi = prime_count;         /* [lo,hi) */
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] <= value)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo == 0) return SIZE_MAX;
    return lo - 1;
}

/* ------------------------------------------------------------ */
/* Constant exponent c ≈ 0.567148130202…, the solution of 127^c−113^c=1 */
static const long double C  = 0.567148130202L;
static const long double INV_C = 1.0L / C;

/* Sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n must be ≥ 1; for n==0 return 0 (undefined) */
    if (n == 0) return (seq_t)0u;

    /* Ensure prime table is ready */
    if (!primes_ready) init_primes();

    /* If n is beyond computed primes, result is 0 */
    if ((size_t)n > prime_count) return (seq_t)0u;

    /* Retrieve p_n = A000040(n) */
    const uint32_t p = prime_list[(size_t)n - 1];

    /* Compute upper bound:
       upper = (p^c + 1)^(1/c)
       Use long double arithmetic; no seq_t operations involved. */
    long double val   = powl((long double)p, C) + 1.0L;
    long double upper = powl(val, INV_C);

    /* Guard against floating‑point round‑off by subtracting a tiny
       epsilon before flooring.  The small epsilon keeps us from
       returning a value one less than the true floor. */
    const uint32_t upper_lim = (uint32_t)(upper + 1e-9L);

    /* Find the last prime ≤ upper_lim. */
    size_t upper_idx = last_le_idx(upper_lim);
    if (upper_idx == SIZE_MAX) return (seq_t)0u;

    size_t base_idx = (size_t)n - 1;
    if (upper_idx <= base_idx) return (seq_t)0u;

    return (seq_t)(upper_idx - base_idx);
}
