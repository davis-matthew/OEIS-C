/*
 * A373736  a(n) = largest nondivisor k < n such that A007947(k) | n, or 0 if
 *          k does not exist.
 * https://oeis.org/A373736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = largest k < n such that k ∤ n and rad(k) | n,
 *              else 0 (OEIS sequence A373736)
 *
 * The implementation precomputes the smallest prime factor for each
 * integer up to the maximum n requested.  From that we compute
 * rad(k) for all k efficiently.  Then a(n) is obtained by scanning
 * downward from n‑1 until a suitable k is found.
 * The algorithm is O(n log log n) for the initial sieve and
 * O(n) for the search, which easily satisfies the time constraint
 * for n ≤ 10 000.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- Static data ---------- */

static idx_t   *spf_arr   = NULL;   /* smallest prime factor */
static idx_t   *rad_arr   = NULL;   /* rad(k) for each k */
static idx_t    max_cached = 0;      /* highest index that has been cached */

/* ---------- Helper: build/extend the caches ---------- */

static void ensure_arrays(idx_t needed_max)
{
    if (needed_max <= max_cached)
        return;

    /* Allocate or grow the arrays */
    spf_arr = (idx_t*)realloc(spf_arr, sizeof(idx_t) * (needed_max + 1));
    rad_arr = (idx_t*)realloc(rad_arr, sizeof(idx_t) * (needed_max + 1));
    if (!spf_arr || !rad_arr)
        abort(); /* memory exhausted */

    /* Initialise spf: by default, each number is its own smallest prime factor */
    for (idx_t i = max_cached + 1; i <= needed_max; ++i)
        spf_arr[i] = i;

    /* Sieve to compute smallest prime factors */
    for (idx_t i = 2; i * i <= needed_max; ++i) {
        if (spf_arr[i] == i) {          /* i is prime */
            for (idx_t j = i * i; j <= needed_max; j += i)
                if (spf_arr[j] == j)
                    spf_arr[j] = i;
        }
    }

    /* Compute rad(k) for all new indices */
    rad_arr[0] = 0;
    rad_arr[1] = 1;
    for (idx_t i = 2; i <= needed_max; ++i) {
        idx_t x   = i;
        idx_t r   = 1;
        idx_t last_p = 0;
        while (x > 1) {
            idx_t p = spf_arr[x];
            if (p != last_p) {           /* new distinct prime factor */
                r *= p;
                last_p = p;
            }
            while (x % p == 0)
                x /= p;
        }
        rad_arr[i] = r;
    }

    max_cached = needed_max;
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n < 2)            /* a(1) = 0 by definition */
        return 0;

    /* Ensure we have rad values for all k < n */
    ensure_arrays(n);

    /* Search for the largest non‑divisor k with rad(k) | n */
    for (idx_t k = n - 1; k > 0; --k) {
        if (n % k == 0)                   /* skip divisors of n */
            continue;

        idx_t r = rad_arr[k];
        if (n % r == 0)                   /* rad(k) divides n */
            return (seq_t)k;              /* found the largest such k */
    }

    return 0;                              /* no suitable k exists */
}
