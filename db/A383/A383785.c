/*
 * A383785  Numbers not occurring as norms of vectors in any regular planar
 *          lattice.
 * https://oeis.org/A383785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 7374, u32 n <= 7374, u64 n <= 7374,
 *            u128 n <= 7374, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383785: numbers not representable as i^2 + j^2
   nor as i^2 – i*j + j^2 (norms of vectors in any regular planar
   lattice).  The function returns the n-th such number, 1‑based. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------- */
/* Integer square root for uint64_t                                   */
/* ----------------------------------------------------------------- */
static inline idx_t
isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) { ans = mid; lo = mid + 1; }
        else          { hi = mid - 1; }
    }
    return ans;
}

/* ----------------------------------------------------------------- */
/* Prime sieve: dynamic list of primes.                              */
/* ----------------------------------------------------------------- */
static idx_t  *prime_list    = NULL;   /* primes themselves         */
static size_t  prime_count   = 0;      /* number of primes stored   */
static size_t  prime_cap     = 0;      /* allocated capacity        */

static void
ensure_primes_up_to(idx_t limit)
{
    if (prime_count == 0) {                /* initialise with 2       */
        prime_cap = 16;
        prime_list = (idx_t*)malloc(prime_cap * sizeof(idx_t));
        prime_list[0] = 2;
        prime_count = 1;
    }

    idx_t last = prime_list[prime_count - 1];
    if (last >= limit) return;

    idx_t cand = last + 1;
    if ((cand & 1) == 0) ++cand;           /* start with odd          */

    for (; cand <= limit; cand += 2) {
        bool is_prime = true;
        idx_t sq = isqrt_u64(cand);
        for (size_t i = 0; i < prime_count && prime_list[i] <= sq; ++i) {
            if (cand % prime_list[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_list = (idx_t*)realloc(prime_list, prime_cap * sizeof(idx_t));
            }
            prime_list[prime_count++] = cand;
        }
    }
}

/* ----------------------------------------------------------------- */
/* Test whether m can be written as i^2 + j^2  (sum of two squares)  */
/* or as i^2 - i*j + j^2  (Eisenstein norm).                         */
/* ----------------------------------------------------------------- */
static bool
is_representable(idx_t m)
{
    if (m == 0) return true;            /* 0 is representable       */

    /* ------------------------------------- */
    /* 1. Sum of two squares                  */
    /* ------------------------------------- */
    bool sum_two_squares = true;
    /* ------------------------------------- */
    /* 2. Eisenstein norm                    */
    /* ------------------------------------- */
    bool eisenstein = true;

    /* factorisation (trial division) */
    idx_t n = m;
    idx_t lim = isqrt_u64(n);
    ensure_primes_up_to(lim);

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= n; ++i) {
        idx_t p = prime_list[i];
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) { n /= p; ++exp; }

            /* sum of two squares: p ≡ 3 (mod 4) must have even exponent  */
            if ((p & 3) == 3 && (exp & 1)) sum_two_squares = false;

            /* Eisenstein norm: p ≡ 2 (mod 3) must have even exponent  */
            if ((p % 3) == 2 && (exp & 1)) eisenstein = false;
        }
    }
    if (n > 1) { /* remaining prime factor */
        idx_t p = n;
        /* sum of two squares */
        if ((p & 3) == 3) sum_two_squares = false;
        /* Eisenstein norm */
        if ((p % 3) == 2) eisenstein = false;
    }

    return sum_two_squares || eisenstein;
}

/* ----------------------------------------------------------------- */
/* Cache of sequence elements                                       */
/* ----------------------------------------------------------------- */
static seq_t *seq_cache = NULL;  /* array holding found terms      */
static size_t  cache_len = 0;    /* number of terms stored         */
static size_t  cache_cap = 0;    /* current capacity               */
static idx_t   next_candidate = 1; /* next integer to test (inclusive) */

/* ----------------------------------------------------------------- */
/* Public sequence function                                          */
/* ----------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* Initialise cache on first call */
    if (seq_cache == NULL) {
        cache_cap = 128;
        seq_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_candidate = 1;
    }

    /* Extend cache until we have at least n terms */
    while (cache_len < n) {
        /* Search for the next non‑representable integer */
        while (is_representable(next_candidate)) ++next_candidate;

        /* Append it to cache */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            seq_cache = (seq_t*)realloc(seq_cache, cache_cap * sizeof(seq_t));
        }
        seq_cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return seq_cache[n - 1];
}
