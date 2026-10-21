/*
 * A258626  Number of length n 1..(3+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5 or 7.
 * https://oeis.org/A258626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n arrays over {1,2,3,4} * such that
 *            every leading partial sum is divisible by 2, 3, 5, or 7. * The
 *            function computes the value by dynamic programming modulo *
 *            lcm(2,3,5,7)=210. Results are cached for repeated calls
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#ifndef MOD
#define MOD 210   /* lcm(2,3,5,7) */
#endif

/* static tables for dynamic programming */
static bool allowed[MOD];
static seq_t dp[MOD];
static seq_t next_vec[MOD];

/* highest index processed so far */
static idx_t current_n = 0;

/* cache of a(1)..a(max)  (cache[0] unused) */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* initialise the table of admissible residues modulo 210 */
static void init_allowed(void)
{
    for (int i = 0; i < MOD; ++i)
        allowed[i] = (i % 2 == 0) || (i % 3 == 0) ||
                     (i % 5 == 0) || (i % 7 == 0);
}

/* grow the cache array to hold values up to n */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;          /* we store indices 0..n */
    if (need <= cache_size)
        return;
    size_t new_size = cache_size ? cache_size << 1 : 64;
    while (new_size < need)
        new_size <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1);                    /* out of memory */
    cache = tmp;
    cache_size = new_size;
}

/* main sequence function */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        init_allowed();
        for (int i = 0; i < MOD; ++i) dp[i] = 0;
        dp[0] = 1;                        /* empty sum = 0 */
    }

    if (n == 0) return 0;                 /* not used, but safe */

    if (n <= current_n)
        return cache[n];

    ensure_cache(n);

    for (idx_t step = current_n + 1; step <= n; ++step) {
        /* reset next_vec */
        for (int i = 0; i < MOD; ++i)
            next_vec[i] = 0;

        /* transition from each residue */
        for (int s = 0; s < MOD; ++s) {
            seq_t val = dp[s];
            if (!val) continue;

            int ns = s + 1; if (ns >= MOD) ns -= MOD;
            if (allowed[ns]) next_vec[ns] += val;

            ns = s + 2; if (ns >= MOD) ns -= MOD;
            if (allowed[ns]) next_vec[ns] += val;

            ns = s + 3; if (ns >= MOD) ns -= MOD;
            if (allowed[ns]) next_vec[ns] += val;

            ns = s + 4; if (ns >= MOD) ns -= MOD;
            if (allowed[ns]) next_vec[ns] += val;
        }

        /* move to next step */
        for (int i = 0; i < MOD; ++i)
            dp[i] = next_vec[i];

        /* total number of valid arrays of this length */
        seq_t total = 0;
        for (int i = 0; i < MOD; ++i)
            total += dp[i];

        cache[step] = total;
    }

    current_n = n;
    return cache[n];
}
