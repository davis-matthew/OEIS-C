/*
 * A303759  Number of times the largest prime power factor of n (A034699) is
 *          largest prime power factor for numbers <= n; a(1) = 1.
 * https://oeis.org/A303759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ m ≤ n : L(m) = L(n) }, where L(m) is the largest
 *            prime‑power dividing m (A034699)
 * Range:     u8 n <= 943, u16 n <= 9182, u32 n <= 9182, u64 n <= 9182,
 *            u128 n <= 9182, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static data structures used for memoisation. */
static seq_t *largest_arr = NULL;          /* largest_arr[m] = L(m)          */
static size_t *freq       = NULL;          /* freq[x] = count of numbers  ≤ current n with L = x */
static uint64_t *spf      = NULL;          /* smallest prime factor array   */
static idx_t spf_size     = 0;             /* size of spf array (1 + max value) */
static idx_t cached_max   = 0;             /* largest n for which data is computed */

/* Simple sieve building the SPF array up to `limit`. */
static void build_spf(idx_t limit)
{
    uint64_t *new_spf = (uint64_t*)realloc(spf, (limit + 1) * sizeof(uint64_t));
    if (!new_spf) exit(1);
    spf = new_spf;
    for (idx_t i = 0; i <= limit; ++i) spf[i] = i;
    if (limit >= 1) spf[1] = 1;
    for (idx_t i = 2; i <= limit / i; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
    spf_size = limit + 1;
}

/* Return the largest prime‑power factor of `n`. Works for n≥1. */
static seq_t largest_prime_power(idx_t n)
{
    if (n == 1) return (seq_t)1;
    uint64_t temp = (uint64_t)n;
    seq_t max_pow = 1;
    while (temp > 1) {
        uint64_t p = spf[temp];
        seq_t pw = 1;
        while (temp % p == 0) {
            temp /= p;
            pw *= (seq_t)p;
        }
        if (pw > max_pow) max_pow = pw;
    }
    return max_pow;
}

/* Main function requested by OEIS. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1, return 0 */

    /* Expand data structures up to `n` if necessary. */
    if (n > cached_max) {
        idx_t new_max = n;

        /* Expand largest_arr. */
        seq_t *new_largest_arr = (seq_t*)realloc(largest_arr,
                                (new_max + 1) * sizeof(seq_t));
        if (!new_largest_arr) exit(1);
        largest_arr = new_largest_arr;

        /* Expand freq array. */
        size_t *new_freq = (size_t*)realloc(freq,
                                (new_max + 1) * sizeof(size_t));
        if (!new_freq) exit(1);
        /* Initialise new entries to zero. */
        for (idx_t i = cached_max + 1; i <= new_max; ++i) new_freq[i] = 0;
        freq = new_freq;

        /* Ensure SPF array is large enough. */
        if (spf_size < new_max + 1) build_spf(new_max);

        /* Compute L for all new numbers and update freq. */
        for (idx_t m = cached_max + 1; m <= new_max; ++m) {
            seq_t lm = largest_prime_power(m);
            largest_arr[m] = lm;
            freq[(size_t)lm] += 1;
        }
        cached_max = new_max;
    }

    /* Return the frequency for the L of n. */
    seq_t ln = largest_arr[n];
    return (seq_t)freq[(size_t)ln];
}
