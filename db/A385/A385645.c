/*
 * A385645  a(n) is the number of distinct sums of distinct prime powers
 *          dividing n.
 * https://oeis.org/A385645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct sums of distinct prime powers dividing
 *            n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm factors n, builds the set of all distinct prime powers
 * (including 1).  Then it uses a simple subset–sum DP to compute all
 * reachable sums from those values.  The number of reachable sums
 * excluding 0 is the desired value.  This runs in O(m * S) time,
 * where m is the number of distinct prime powers and S is their
 * total sum; for n ≤ 10000 both are very small. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* n is the index, start counting from 1 */
    if (n == 1)
        return (seq_t)1;

    /* Factor n into primes and exponents */
    uint64_t primes[8];
    uint64_t exps[8];
    size_t cnt = 0;
    uint64_t tmp = n;

    if ((tmp & 1ULL) == 0) {  /* handle factor 2 */
        primes[cnt] = 2;
        exps[cnt] = 0;
        while (tmp % 2ULL == 0) {
            tmp /= 2ULL;
            exps[cnt]++;
        }
        cnt++;
    }

    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            primes[cnt] = p;
            exps[cnt] = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exps[cnt]++;
            }
            cnt++;
        }
    }

    if (tmp > 1) {
        primes[cnt] = tmp;
        exps[cnt] = 1;
        cnt++;
    }

    /* Count total number of distinct prime powers (including 1) */
    size_t num_vals = 1;  /* for 1 */
    for (size_t i = 0; i < cnt; ++i)
        num_vals += exps[i];

    /* Allocate array for prime powers */
    uint64_t *vals = (uint64_t *)malloc(num_vals * sizeof(uint64_t));
    size_t pos = 0;
    vals[pos++] = 1;  /* value 1 (p^0) appears only once */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t val = 1;
        for (uint64_t k = 1; k <= exps[i]; ++k) {
            val *= primes[i];
            vals[pos++] = val;
        }
    }

    /* Compute total sum of all prime powers */
    idx_t sum_total = 0;
    for (size_t i = 0; i < num_vals; ++i)
        sum_total += (idx_t)vals[i];

    /* DP array for subset sums */
    size_t dp_size = (size_t)sum_total + 1;
    uint8_t *dp = (uint8_t *)calloc(dp_size, sizeof(uint8_t));
    dp[0] = 1;  /* empty subset sum */

    for (size_t i = 0; i < num_vals; ++i) {
        idx_t v = (idx_t)vals[i];
        for (idx_t s = sum_total; s >= v; --s) {
            if (dp[s - v])
                dp[s] = 1;
        }
    }

    /* Count non‑zero reachable sums */
    seq_t result = 0;
    for (size_t s = 1; s <= (size_t)sum_total; ++s)
        if (dp[s])
            result++;

    /* Clean up */
    free(vals);
    free(dp);

    return result;
}
