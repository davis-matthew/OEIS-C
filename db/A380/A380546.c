/*
 * A380546  Cumulative sum of the greatest prime in the minimal Goldbach
 *          partition for 2*n, n>=2.
 * https://oeis.org/A380546
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(2) + Σ_{k=3..n} (2*k – p_min(k)) where p_min(k) is the
 *            smallest prime such that (2*k – p_min(k)) is prime. This is the
 *            cumulative sum of the greatest prime in the minimal Goldbach
 *            partition of 2*n (n ≥ 2). a(2)=2
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    static seq_t *cum = NULL;
    static size_t cum_size = 0;
    static uint8_t *isPrime = NULL;
    static size_t prime_size = 0;
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;

    /* ---------- Ensure cache up to n ---------- */
    if (cum_size <= n) {
        idx_t limit = 2 * n;          /* up to 2*n, inclusive */

        /* Build / rebuild prime sieve if necessary */
        if (prime_size <= limit) {
            size_t sz = (size_t)limit + 1;
            uint8_t *tmp = (uint8_t *)malloc(sz);
            for (size_t i = 0; i < sz; ++i) tmp[i] = 1;
            tmp[0] = tmp[1] = 0;
            for (size_t i = 2; i * i <= limit; ++i) {
                if (tmp[i]) {
                    for (size_t j = i * i; j <= limit; j += i) tmp[j] = 0;
                }
            }
            size_t est = limit / 2 + 1;
            uint64_t *plist = (uint64_t *)malloc(est * sizeof(uint64_t));
            size_t cnt = 0;
            for (size_t i = 2; i <= limit; ++i) {
                if (tmp[i]) plist[cnt++] = i;
            }
            free(isPrime);
            free(prime_list);
            isPrime   = tmp;
            prime_list = plist;
            prime_cnt  = cnt;
            prime_size = limit;
        }

        /* Extend cumulative array */
        seq_t *new_cum = (seq_t *)realloc(cum, (n + 1) * sizeof(seq_t));
        cum = new_cum;
        for (size_t i = cum_size; i <= n; ++i) {
            if (i == 0 || i == 1) {
                cum[i] = 0;
                continue;
            }
            idx_t even = 2 * i;
            size_t half = even / 2;
            uint64_t p = 0;
            for (size_t j = 0; j < prime_cnt; ++j) {
                p = prime_list[j];
                if (p > half) break;
                if (isPrime[even - p]) break;
            }
            seq_t greatest = (seq_t)(even - p);
            if (i == 2) cum[i] = greatest;
            else cum[i] = cum[i - 1] + greatest;
        }
        cum_size = n + 1;
    }
    return cum[n];
}
