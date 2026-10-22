/*
 * A401013  Maximum number of distinct prime divisors of n+k greater than k,
 *          over all integers k >= 0.
 * https://oeis.org/A401013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = max over k >= 0 of the number of distinct prime divisors
 *            of (n + k) that are greater than k. When k = 0 this gives the
 *            number of distinct prime divisors of n (omega(n)). This function
 *            computes the value for any n >= 1 by enumerating k up to n and
 *            using a sieve of smallest prime factors for...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Static storage for the smallest prime factor table */
    static uint64_t *spf = NULL;
    static size_t spf_size = 0;

    /* Build SPF table up to 2*n + 20 (to be safe) */
    size_t limit = 2 * ((size_t)n) + 20;
    if (spf_size < limit) {
        free(spf);
        spf = (uint64_t*)malloc(limit * sizeof(uint64_t));
        if (!spf) return 0;  /* allocation failed */
        memset(spf, 0, limit * sizeof(uint64_t));

        for (size_t i = 2; i < limit; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                if (i <= limit / i) {
                    for (size_t j = i * i; j < limit; j += i) {
                        if (spf[j] == 0) spf[j] = i;
                    }
                }
            }
        }
        spf_size = limit;
    }

    seq_t maxcnt = 0;

    for (idx_t k = 0; k <= n; ++k) {
        idx_t m = n + k;
        seq_t cnt = 0;
        idx_t tmp = m;

        while (tmp > 1) {
            uint64_t p = spf[(size_t)tmp];
            if (p > (uint64_t)k) cnt++;
            while (tmp % (idx_t)p == 0) tmp /= (idx_t)p;
        }
        if (cnt > maxcnt) maxcnt = cnt;
    }

    return maxcnt;
}
