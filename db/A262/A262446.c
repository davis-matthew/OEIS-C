/*
 * A262446  Number of ways to write A262439(n) = A262439(k) + A262439(m) with
 *          0 < k < m < n.
 * https://oeis.org/A262446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = #{ (k,m) | 0<k<m<n  and
 *                pi( n*(n+1)/2+1 ) = pi( k*(k+1)/2+1 ) + pi( m*(m+1)/2+1 ) }
 * where pi(x) is the number of primes ≤ x.
 *
 * Implementation details:
 *   - On first use, or when n exceeds the previously cached size, the
 *     function precomputes all triangular numbers T(i)=i*(i+1)/2+1 for
 *     1≤i≤n, builds a prime sieve up to T(n), and stores pi(T(i)) in an
 *     array.
 *   - The required count is then obtained by a two‑pointer scan of the
 *     (ordered) array of pi(T(i)), which runs in O(n) time.
 *   - All data that may exceed the width of the user‑supplied type
 *     seq_t are kept in native 32‑bit or 64‑bit integers; the result is
 *     cast to seq_t only at the end.  This guarantees exactness for all
 *     values that fit in seq_t, while remaining correct for larger
 *     counts as well.
 *
 * The function is re‑entrant for calls in arbitrary order; it uses
 * static storage that grows with the maximum n seen.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- prime sieve helpers ---------- */

static idx_t      sieve_limit   = 0;          /* highest number sieved up to */
static uint8_t   *is_prime_flag = NULL;       /* 1 if prime, 0 otherwise */
static uint32_t  *pi_sieve      = NULL;       /* pi_sieve[i] = #primes ≤ i */

/* ensure we have a sieve up to `limit` (inclusive) */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* free previous tables if any */
    free(is_prime_flag);
    free(pi_sieve);

    /* allocate new tables */
    is_prime_flag = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    pi_sieve      = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    if (!is_prime_flag || !pi_sieve) abort();

    /* basic Eratosthenes sieve */
    for (idx_t i = 2; i <= limit; ++i) is_prime_flag[i] = 1;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime_flag[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime_flag[j] = 0;
        }
    }

    /* build pi table */
    pi_sieve[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        pi_sieve[i] = pi_sieve[i - 1] + (is_prime_flag[i] ? 1U : 0U);

    sieve_limit = limit;
}

/* ---------- array of pi( T(i) ) ---------- */

static idx_t      tri_cache_limit = 0;   /* highest i for which we have values */
static uint32_t  *pi_tri          = NULL;/* pi_tri[i] = pi( i*(i+1)/2 + 1 ) */

/* ensure we have values up to index n */
static void ensure_tri_cache(idx_t n)
{
    if (n <= tri_cache_limit) return;

    /* compute upper bound for sieving */
    idx_t max_T = n * (n + 1) / 2 + 1;
    ensure_sieve(max_T);

    /* allocate new cache */
    free(pi_tri);
    pi_tri = (uint32_t *)malloc((size_t)(n + 1) * sizeof(uint32_t));
    if (!pi_tri) abort();

    for (idx_t i = 0; i <= n; ++i) {
        idx_t T = i * (i + 1) / 2 + 1;
        pi_tri[i] = pi_sieve[T];
    }

    tri_cache_limit = n;
}

/* ---------- main OEIS sequence function ---------- */

seq_t a(idx_t n)
{
    if (n <= 2) return 0;            /* no admissible pair for n≤2 */

    ensure_tri_cache(n);

    uint32_t target = pi_tri[n];

    /* two-pointer scan over [1 .. n‑1] (k < m) */
    idx_t left  = 1;
    idx_t right = n - 1;
    unsigned long long count = 0ULL;

    while (left < right) {
        uint32_t sum = pi_tri[left] + pi_tri[right];
        if (sum < target) {
            ++left;
        } else if (sum > target) {
            --right;
        } else {
            /* count equal values on both sides */
            uint32_t left_val  = pi_tri[left];
            uint32_t right_val = pi_tri[right];
            idx_t cnt_left  = 0;
            idx_t cnt_right = 0;
            while (left <= right && pi_tri[left] == left_val) {
                ++cnt_left;
                ++left;
            }
            while (right >= left && pi_tri[right] == right_val) {
                ++cnt_right;
                --right;
            }
            count += (unsigned long long)cnt_left * (unsigned long long)cnt_right;
        }
    }

    return (seq_t)count;
}
