/*
 * A254761  One half of the fundamental positive solution y = y1(n) of the
 *          first class of the Pell equation x^2 - 2*y^2 = A007519(n), n >= 1
 *          (primes congruent to 1 mod 8).
 * https://oeis.org/A254761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254761: one half of the fundamental positive solution y of
 * the Pell equation x^2 - 2*y^2 = p, where p is the n-th prime congruent
 * to 1 (mod 8).  That is, a(n) = y/2 where (x,y) is the smallest
 * positive solution of x²‑2y² = p.  Pseudocode implementation that
 * generates the primes 1 (mod 8) on demand and then finds the minimal
 * y by trial.  The routine works for any of the supported seq_t
 * types; intermediate arithmetic uses uint64_t so that a(n) is exact
 * whenever it fits in seq_t.  */
 
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Dynamic array helpers for uint64_t */
static void grow_uint64_array(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        *arr = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        *cap = new_cap;
    }
}

/* Small integer square‑root routine for uint64_t values.
   Uses 128‑bit multiplication to avoid overflow. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = (uint64_t)1 << 32;
    uint64_t res = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = ( __uint128_t )mid * mid;
        if (sq <= n) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

/* Prime storage structures */
static uint64_t *primes_all = NULL;      /* all primes discovered */
static size_t     primes_all_cap = 0;
static size_t     primes_all_cnt = 0;

static uint64_t *primes_1mod8 = NULL;    /* primes ≡ 1 (mod 8) */
static size_t     primes_1_cap   = 0;
static size_t     primes_1_cnt   = 0;

static uint64_t next_candidate = 2;      /* next number to test */

/* Test whether n is prime using the primes discovered so far */
static bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_all_cnt; ++i) {
        uint64_t p = primes_all[i];
        if (p > n / p) break;           /* p*p > n */
        if (n % p == 0) return false;
    }
    return true;
}

/* Ensure that the list of primes ≡ 1 (mod 8) contains at least 'n' entries */
static void ensure_prime_1mod8(size_t n)
{
    while (primes_1_cnt < n) {
        uint64_t cand = next_candidate;
        const bool isp = is_prime_candidate(cand);
        if (isp) {
            grow_uint64_array(&primes_all, &primes_all_cap, primes_all_cnt + 1);
            primes_all[primes_all_cnt++] = cand;
            if ((cand & 7ULL) == 1) {
                grow_uint64_array(&primes_1mod8, &primes_1_cap,
                                  primes_1_cnt + 1);
                primes_1mod8[primes_1_cnt++] = cand;
            }
        }
        /* advance candidate: skip even numbers after 2 */
        if (cand == 2) next_candidate = 3;
        else            next_candidate += 2;
    }
}

/* ------------------------------------------------------------------------- */
/*  Public sequence function                                               */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n = 0 */

    /* Ensure that the n-th prime of form 1 (mod 8) is available */
    ensure_prime_1mod8((size_t)n);

    uint64_t p = primes_1mod8[n - 1];

    /* Find smallest y such that p + 2*y*y is a perfect square. */
    uint64_t y = 1;
    while (true) {
        uint64_t v = 2ULL * y * y + p;
        uint64_t s = isqrt_u64(v);
        if (s * s == v) break;
        ++y;
    }

    /* For such primes, y is even – return y/2. */
    return (seq_t)(y / 2);
}
