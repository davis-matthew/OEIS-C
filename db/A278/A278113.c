/*
 * A278113  Triangle T(n,k) = A278112(n,A000040(k)) for 1 <= k <= A278114(n),
 *          read by rows.
 * https://oeis.org/A278113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = floor(r * sqrt(2 / prime(k))) where (r,k) are the row and
 *            column of the triangle entry for the index n. Equivalent integer
 *            formula: a(n) = floor_sqrt( (2*r^2) / prime(k) )
 * Range:     u8 n <= 59, u16 n <= 9929, u32 n <= 9929, u64 n <= 9929,
 *            u128 n <= 9929, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Forward declaration of the user function */
seq_t a(idx_t n);

/* Global data for memoisation */
static size_t cum_cap = 0;
static size_t cum_len = 0;
static idx_t *cum = NULL;           /* cumulative counts per row */

static size_t primes_cap = 0;
static size_t primes_len = 0;
static idx_t *primes = NULL;        /* list of primes, 1‑based index */
static idx_t prime_limit = 0;       /* highest number up to which we have sieved */

/* ------------------------------------------------------------ */
/* Utility: integer square root using binary search             */
static seq_t isqrt_seq(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;
    seq_t best = 0;

    while (lo <= hi) {
        seq_t mid = lo + (hi - lo) / 2;
        seq_t sq  = mid * mid;

        if (sq <= x) {
            best = mid;
            lo   = mid + 1;
        } else {
            if (mid == 0) break;
            hi   = mid - 1;
        }
    }
    return best;
}

/* ------------------------------------------------------------ */
/* Build primes up to 'limit' (inclusive) and populate array     */
static void ensure_prime_limit(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* Allocate temporary boolean array for sieve */
    unsigned char *is_prime = (unsigned char *)malloc((size_t)(limit + 1));
    if (!is_prime) return;               /* Out of memory – give up */

    memset(is_prime, 1, (size_t)(limit + 1));
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Replace old prime list */
    free(primes);
    primes_len = 0;
    primes_cap = (size_t)(limit / 10) + 10;
    primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    if (!primes) {
        free(is_prime);
        return;
    }

    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (primes_len >= primes_cap) {
                primes_cap *= 2;
                primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
            }
            primes[primes_len++] = i;
        }
    }

    prime_limit = limit;
    free(is_prime);
}

/* ------------------------------------------------------------ */
/* Count of primes <= 'x' using binary search on the prime list */
static idx_t prime_pi(idx_t x)
{
    idx_t lo = 0, hi = primes_len;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (primes[mid] <= x) lo = mid + 1;
        else                  hi = mid;
    }
    return lo;   /* number of primes <= x */
}

/* ------------------------------------------------------------ */
/* Ensure that cumulative counts have been produced up to at least index 'n' */
static void ensure_row(idx_t n)
{
    while (cum_len == 0 || cum[cum_len - 1] < n) {
        idx_t r = (idx_t)(cum_len + 1);                 /* next row number */
        idx_t limit = 2 * r * r;

        ensure_prime_limit(limit);
        idx_t count = prime_pi(limit);                  /* columns in row r */

        idx_t new_cum = (cum_len == 0) ? count : cum[cum_len - 1] + count;

        if (cum_len >= cum_cap) {
            cum_cap = (cum_cap ? cum_cap * 2 : 8);
            cum = (idx_t *)realloc(cum, cum_cap * sizeof(idx_t));
        }
        cum[cum_len++] = new_cum;                       /* cumulative count */
    }
}

/* ------------------------------------------------------------ */
/* Main function to compute the sequence term a(n)              */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of range – not used by OEIS */

    ensure_row(n);

    /* Locate the row containing index n */
    idx_t lo = 0, hi = cum_len;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cum[mid] >= n) hi = mid;
        else                lo = mid + 1;
    }

    idx_t row       = lo + 1;                                       /* 1‑based */
    idx_t prev_cum  = (lo == 0) ? 0 : cum[lo - 1];
    idx_t k         = n - prev_cum;                                 /* 1‑based column */
    idx_t prime     = primes[k - 1];

    /* Compute floor_sqrt( (2 * row^2) / prime ) */
    seq_t rr  = (seq_t)row;
    seq_t num = rr * rr;            /* row^2 */
    num = num * (seq_t)2;
    num = num / prime;              /* integer division */
    seq_t val = isqrt_seq(num);

    return val;
}
