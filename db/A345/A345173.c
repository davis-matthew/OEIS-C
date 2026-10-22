/*
 * A345173  Numbers whose multiset of prime factors is separable but has no
 *          alternating permutation.
 * https://oeis.org/A345173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers whose multiset of prime factors is separable but has no
 *            alternating permutation
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   dfs_alt, has_alternating
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/dfs_alt.h"
#include "../utils/has_alternating.h"

static uint32_t *g_spf = NULL;
static int      *g_prime_index = NULL;
static size_t    g_prime_count = 0;
static const idx_t LIMIT = 8000000;   /* upper bound for the search */
static bool      g_inited = false;

static void
init_sieve(void)
{
    size_t limit = LIMIT;
    g_spf        = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    g_prime_index= (int*)      malloc((limit + 1) * sizeof(int));
    memset(g_spf,        0, (limit + 1) * sizeof(uint32_t));
    memset(g_prime_index,0, (limit + 1) * sizeof(int));

    size_t i;
    for (i = 2; i <= limit; ++i) {
        if (g_spf[i] == 0) {          /* i is prime */
            g_spf[i] = (uint32_t)i;
            g_prime_index[i] = (int)(g_prime_count + 1);  /* 1‑based index */
            g_prime_count++;
            if (i * i <= limit) {
                size_t j;
                for (j = i * i; j <= limit; j += i)
                    if (g_spf[j] == 0)
                        g_spf[j] = (uint32_t)i;
            }
        }
    }
    g_inited = true;
}

seq_t a(idx_t n)
{
    if (!g_inited)
        init_sieve();

    idx_t count = 0;

    for (uint32_t m = 2; m <= LIMIT; ++m) {
        int uniq[64];
        int freq[64];
        int uniq_cnt = 0;
        int k = 0;          /* total number of prime factors (with multiplicity) */

        uint32_t temp = m;
        while (temp > 1) {
            uint32_t p = g_spf[temp];
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            int idx = g_prime_index[p];
            int j;
            for (j = 0; j < uniq_cnt; ++j)
                if (uniq[j] == idx)
                    break;
            if (j < uniq_cnt)
                freq[j] += e;
            else {
                uniq[uniq_cnt] = idx;
                freq[uniq_cnt] = e;
                uniq_cnt++;
            }
            k += e;
        }
        if (k == 0)
            continue;                 /* m == 1, skip */

        /* check separability: max frequency <= ceil(k/2) */
        int maxfreq = 0;
        for (int i = 0; i < uniq_cnt; ++i)
            if (freq[i] > maxfreq)
                maxfreq = freq[i];
        if (maxfreq > (k + 1) / 2)
            continue;                 /* not separable */

        /* check alternating */
        if (!has_alternating(uniq, freq, uniq_cnt, k)) {
            count++;
            if (count == n)
                return (seq_t)m;
        }
    }
    /* if the global limit is insufficient */
    return 0;
}
