/*
 * A245508  Smallest double square (cf. A001105) greater than n-th prime.
 * https://oeis.org/A245508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A245508: Smallest double square (2*k^2) ≥ k-th prime.
 * Formula: a(n) = 2 * ceil( sqrt( (prime(n)+1)/2 ) )^2
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* --------- internal helpers --------- */

/* small sieve to generate primes up to the given index */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

/* Estimate an upper bound on the n-th prime (n>=6). */
static idx_t estimate_limit(idx_t n)
{
    if (n < 6) return 15;
    double nn = (double)n;
    double lim = nn * (log(nn) + log(log(nn))) + 3.0;
    return (idx_t)lim;
}

/* Ensure the first 'need' primes are available, extending the sieve if necessary. */
static void build_primes_upto(idx_t need)
{
    if (primes_cnt >= (size_t)need) return;

    idx_t limit = estimate_limit(need);
    if (limit <= 1) limit = 2;

    /* is_comp[i] == 1 => composite, 0 => prime candidate */
    char *is_comp = (char *)calloc((size_t)(limit + 1), 1);
    if (!is_comp) return; /* allocation failure -- give up silently */

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (cnt == primes_cap) {
                primes_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) { free(is_comp); return; }
                primes = tmp;
            }
            primes[cnt++] = (uint64_t)i;

            if ((uint64_t)i * (uint64_t)i <= (uint64_t)limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    primes_cnt = cnt;
    free(is_comp);
}

/* Return the k-th prime (1‑based). Extends the sieve if needed. */
static uint64_t get_prime_u64(idx_t k)
{
    if ((size_t)k > primes_cnt) build_primes_upto(k);
    return primes[(size_t)k - 1];
}

/* Floor integer square root of a 64‑bit unsigned integer. */
static uint64_t floor_sqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;  /* max sqrt fits in 32 bits */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

/* Ceil integer square root of a 64‑bit unsigned integer. */
static uint64_t ceil_sqrt_u64(uint64_t n)
{
    uint64_t r = floor_sqrt_u64(n);
    if ((unsigned __int128)r * r < n) ++r;
    return r;
}

/* --------- public function --------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* for safety, though offset is 1 */

    /* 1‑based prime */
    uint64_t p = get_prime_u64(n);

    /* compute ceil(sqrt((p+1)/2)) */
    uint64_t m = (p + 1) / 2;       /* integer arithmetic gives ceil(p/2) */
    uint64_t k = ceil_sqrt_u64(m);

    /* a(n) = 2 * k^2 */
    unsigned __int128 val = (unsigned __int128)2 * k * k;
    return (seq_t)val;
}
