/*
 * A295014  Prime time numbers on 6-digit clocks: numbers of the form HMMSS
 *          where H, MM, SS are primes, H < 24, MM and SS < 60.
 * https://oeis.org/A295014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2601, u64 n <= 2601, u128 n <= 2601,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force generation of the prime time numbers HMMSS where
   H, MM, SS are primes <24, <60 respectively.
   The values are pre‑computed once, sorted, and indexed.
   The function returns a(n) for n ≥ 1. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Comparator for qsort on seq_t values */
static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* seq_t a(idx_t n) */
seq_t a(idx_t n)
{
    /* static cache of the sorted sequence */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static bool inited = false;

    if (!inited)
    {
        /* primes for hour (H) and minute/second (MM, SS) */
        static const uint64_t primes_hour[]   = {2,3,5,7,11,13,17,19,23};
        static const uint64_t primes_minsec[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59};

        const size_t hour_cnt  = sizeof(primes_hour)   / sizeof(primes_hour[0]);
        const size_t mins_cnt  = sizeof(primes_minsec) / sizeof(primes_minsec[0]);

        /* total number of combinations */
        cache_size = hour_cnt * mins_cnt * mins_cnt;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);

        size_t idx = 0;
        for (size_t h = 0; h < hour_cnt; ++h)
        {
            uint64_t H = primes_hour[h];
            for (size_t m = 0; m < mins_cnt; ++m)
            {
                uint64_t M = primes_minsec[m];
                for (size_t s = 0; s < mins_cnt; ++s)
                {
                    uint64_t S = primes_minsec[s];
                    cache[idx++] = (seq_t)(H * 10000ULL + M * 100ULL + S);
                }
            }
        }

        /* Sort the sequence */
        qsort(cache, cache_size, sizeof(seq_t), cmp_seq_t);

        inited = true;
    }

    if (n < 1 || (size_t)(n - 1) >= cache_size)
        return 0;        /* undefined for out‑of‑range n */

    return cache[n - 1];
}
