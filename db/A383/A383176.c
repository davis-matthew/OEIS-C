/*
 * A383176  If p = A002313(n) is a prime such that p = x^2 + y^2, then a(n) is
 *          the largest integer k that satisfies x^2 + y^2 - k*x*y > 0.
 * https://oeis.org/A383176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((p-1)/(x*y)) where p is the nth prime of A002313
 *            (primes ≡ 1 (mod 4) plus 2) and p = x^2 + y^2 (unique
 *            representation with integers x >= y > 0)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* integer square root for uint64_t ------------------------------------- */
static uint64_t floor_sqrt_u64(uint64_t n) {
    uint64_t lo = 0, hi = n;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        else if (sq < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

/* dynamic array resize helper ------------------------------------------ */
static void ensure_capacity(uint64_t **arr, size_t *cap, size_t needed) {
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        uint64_t *tmp = (uint64_t*)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* global prime storage --------------------------------------------------- */
static uint64_t *all_primes   = NULL;
static size_t     all_cnt     = 0;
static size_t     all_cap     = 0;

static uint64_t *four_primes  = NULL;
static size_t     four_cnt    = 0;
static size_t     four_cap    = 0;

static uint64_t   current_candidate = 3;
static bool       initialized = false;

/* sieve primes until we have at least target four-primes (mod 4 = 1 or 2) */
static void ensure_four_primes(idx_t target) {
    if (!initialized) {
        ensure_capacity(&all_primes, &all_cap, 1);
        ensure_capacity(&four_primes, &four_cap, 1);
        all_primes[all_cnt++] = 2;
        four_primes[four_cnt++] = 2;   /* 2 is included in the sequence */
        initialized = true;
    }
    while (four_cnt < (size_t)target) {
        uint64_t c = current_candidate;
        uint64_t sqrt_c = floor_sqrt_u64(c);
        bool is_prime = true;
        for (size_t i = 0; i < all_cnt && all_primes[i] <= sqrt_c; ++i) {
            if (c % all_primes[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            ensure_capacity(&all_primes, &all_cap, all_cnt + 1);
            all_primes[all_cnt++] = c;
            if (c == 2 || (c & 3) == 1) { /* mod 4 = 1 or 2 (i.e., 2) */
                ensure_capacity(&four_primes, &four_cap, four_cnt + 1);
                four_primes[four_cnt++] = c;
            }
        }
        current_candidate += 2;   /* skip even numbers except 2 */
    }
}

/* main sequence function ------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for 0; not used by OEIS */
    if (n == 1) return (seq_t)1;   /* a(1) = 1 (p = 2) */

    ensure_four_primes(n);
    uint64_t p = four_primes[n - 1];

    /* find the unique representation p = x^2 + y^2 (x >= y) */
    uint64_t x, y;
    uint64_t sqrt_p = floor_sqrt_u64(p);
    for (x = 1; x <= sqrt_p; ++x) {
        __uint128_t xsq = (__uint128_t)x * x;
        if (xsq > p) break;
        uint64_t y2 = p - (uint64_t)xsq;
        y = floor_sqrt_u64(y2);
        if (y * y == y2) break;
    }

    /* largest integer k such that p - k*x*y > 0  ->  k = floor((p-1)/(x*y)) */
    uint64_t xy = x * y;
    uint64_t k  = (p - 1) / xy;

    return (seq_t)k;
}
