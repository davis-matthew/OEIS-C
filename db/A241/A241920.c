/*
 * A241920  After a(1)=1, each n appears A061395(n) times, where A061395 gives
 *          the index of the largest prime factor of n.
 * https://oeis.org/A241920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 802, u16 n <= 8270, u32 n <= 8270, u64 n <= 8270,
 *            u128 n <= 8270, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241920: a(n) is the n‑th term of the sequence obtained by
 * repeating every integer i exactly A061395(i) times,
 * where A061395(i) is the index of the largest prime factor of i.
 * Implementation uses a running cumulative count and a dynamic prime
 * sieve.  The function works for any build (uint8_t … unsigned __int128,
 * arbitrary precision) and supports non‑sequential queries.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* provides void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */

/* ---------- static state used for caching ---------- */
static idx_t  primes_max = 0;      /* maximum index up to which primes are stored */
static uint64_t *primes    = NULL;  /* sorted prime list, 0‑based, size primes_cnt */
static size_t   primes_cnt = 0;    /* number of primes stored */

/* Allocate or grow the prime table up to a given limit.  The primes
 * array is rebuilt each time a larger limit is requested.  This
 * function is idempotent for the same or smaller limits. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= primes_max) return;

    /* rebuild the entire prime list to the new limit */
    free(primes);
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_max = limit;
}

/* Binary search for the index (1‑based) of a given prime p in the
 * global primes array.  It is assumed that p is prime and <= primes_max. */
static seq_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = primes_cnt - 1;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p) {
            return (seq_t)(mid + 1);  /* convert to 1‑based index */
        } else if (primes[mid] < p) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    /* The prime must exist in the table; return 1 as a safe default. */
    return 1;
}

/* Return the index of the largest prime factor of n (n ≥ 1).  For
 * n = 1 this returns 1, treating 1 as appearing once. */
static seq_t get_lpf_index(idx_t n)
{
    if (n <= 1) return 1;

    ensure_primes_upto(n);                /* prime table covers all primes <= n */

    idx_t      temp = n;
    seq_t      best = 0;                  /* index of largest prime factor found */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if ((idx_t)p * (idx_t)p > temp) break;   /* remaining factor is prime */

        if (temp % p == 0) {
            best = prime_index(p);        /* record current prime's index */
            while (temp % p == 0) temp /= p;  /* remove all occurrences */
        }
    }

    if (temp > 1) {                       /* temp itself is prime */
        best = prime_index((uint64_t)temp);
    }

    /* For safety: if something went wrong, default to 1. */
    if (!best) best = 1;
    return best;
}

/* ---------- cumulative count array ---------- */
/* cum[i] contains the total number of terms contributed by
 * integers 1..i.  cum[0] = 0.  cum_len is the last defined index. */
static seq_t *cum      = NULL;
static size_t cum_cap  = 0;
static size_t cum_len  = 0;

/* Grow the cumulative array until cum[cum_len] >= target_n.  This
 * function ensures primes are available for all indices needed. */
static void make_cumulative_up_to(idx_t target_n)
{
    /* initialise first time */
    if (!cum) {
        cum_cap = 64;
        cum     = (seq_t *)calloc(cum_cap, sizeof(seq_t));
        cum[0]  = 0;
        cum_len = 0;
    }

    while (cum_len == 0 || cum[cum_len] < (seq_t)target_n) {
        /* prepare capacity for the next index */
        if (cum_len + 1 >= cum_cap) {
            cum_cap <<= 1;
            cum = (seq_t *)realloc(cum, cum_cap * sizeof(seq_t));
        }

        size_t idx = cum_len + 1;                /* next integer to process */
        seq_t  count = get_lpf_index((idx_t)idx);/* how many times idx appears */
        cum[idx] = cum[idx - 1] + count;         /* cumulative sum */
        cum_len  = idx;
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n == 0, but safe */

    make_cumulative_up_to(n);

    /* binary search for the smallest i with cum[i] >= n */
    size_t lo = 1, hi = cum_len, ans = cum_len;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (cum[mid] >= (seq_t)n) {
            ans = mid;
            if (mid == 0) break;
            hi  = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return (seq_t)ans;
}
