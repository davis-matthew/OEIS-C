/*
 * A262432  Regular triangle read by rows: T(n, k) gives the number of times
 *          that the denominator of sigma(x,-1) (A017666) is equal to k when x
 *          goes from 1 to n.
 * https://oeis.org/A262432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = T(r,k), where r is the unique integer with
 *  r*(r-1)/2 < n <= r*(r+1)/2  and
 *  k = n - r*(r-1)/2.
 *  T(r,k) is the number of integers x (1 ≤ x ≤ r)
 *  whose denominator of sigma(x,-1) equals k.
 *  It equals the number of x such that x / gcd(sigma(x),x) = k.  */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/gcd.h"

/* cache of sigma values  (1‑based indexing) */
static seq_t *sigma = NULL;
static idx_t  sigma_cap = 0;

/* temporary frequency array for the current row */
static idx_t *freq = NULL;
static idx_t  freq_cap = 0;

/*  Ensure sigma[1..limit] is computed.  */
static void
ensure_sigma(idx_t limit)
{
    if (limit <= sigma_cap) return;

    /* free old storage */
    free(sigma);

    /* allocate (limit+1) entries, 1‑based; use malloc + cast for
     * C++ arbitrary‑precision builds.  */
    sigma = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!sigma) exit(1);          /* out of memory */
    /* zero‑initialize */
    memset(sigma, 0, (size_t)(limit + 1) * sizeof(seq_t));

    /* classic harmonic series sieve for sum of divisors */
    for (idx_t d = 1; d <= limit; ++d) {
        seq_t sd = (seq_t)d;
        for (idx_t m = d; m <= limit; m += d) {
            sigma[m] += sd;
        }
    }
    sigma_cap = limit;
}

/*  Ensure freq[0..max_n] is allocated; 0‑index unused.  */
static void
ensure_freq(idx_t max_n)
{
    if (max_n <= freq_cap) {
        /* zero all used cells */
        memset(freq, 0, (size_t)(max_n + 1) * sizeof(idx_t));
        return;
    }
    free(freq);
    freq = (idx_t *)malloc((size_t)(max_n + 1) * sizeof(idx_t));
    if (!freq) exit(1);
    memset(freq, 0, (size_t)(max_n + 1) * sizeof(idx_t));
    freq_cap = max_n;
}

seq_t a(idx_t n)
{
    /* 1. locate the row r that contains the n‑th element */
    idx_t low = 1, high = 1;
    while (1) {
        __uint128_t val = (__uint128_t)high * (high + 1) / 2;
        if (val >= n) break;
        /* grow high exponentially; it will remain small for n ≤ 1e10 */
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* 2. compute column index k (1‑based) within row r */
    __uint128_t prev_sum = (__uint128_t)r * (r - 1) / 2;
    idx_t k = (idx_t)(n - prev_sum);

    /* 3. compute sigma up to r if needed */
    ensure_sigma(r);

    /* 4. prepare frequency array for the row */
    ensure_freq(r);

    /* 5. count occurrences of each denominator */
    for (idx_t x = 1; x <= r; ++x) {
        seq_t sigma_x = sigma[x];
        seq_t g = gcd(sigma_x, (seq_t)x);
        idx_t denom = (idx_t)((seq_t)x / g);   /* denom ≤ x ≤ r */
        freq[denom]++;                         /* frequency count */
    }

    return (seq_t)freq[k];
}
