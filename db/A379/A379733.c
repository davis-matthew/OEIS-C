/*
 * A379733  Number of strict integer partitions of n whose product of parts is
 *          a multiple of n.
 * https://oeis.org/A379733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of strict integer partitions of n whose product of
 *            parts is a multiple of n
 * Range:     u8 n <= 35, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   expo_cap, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Add helper from utils */
#include "../utils/is_prime.h"
#include "../utils/expo_cap.h"

typedef idx_t idx_t;  /* defined by build, just for clarity */
typedef seq_t seq_t;  /* defined by build */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* for safety; sequence is defined for n>=1 */

    /* 1. Factor n into distinct primes and their exponents */
    uint64_t primes[10];
    int exp_needed[10];
    int prime_cnt = 0;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[prime_cnt] = p;
            exp_needed[prime_cnt] = e;
            ++prime_cnt;
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        primes[prime_cnt] = temp;
        exp_needed[prime_cnt] = 1;
        ++prime_cnt;
    }

    /* 2. Mixed-radix parameters for exponent vectors */
    size_t radices[10];
    radices[0] = 1;
    for (int i = 1; i < prime_cnt; ++i)
        radices[i] = radices[i - 1] * (size_t)(exp_needed[i - 1] + 1);
    size_t total_states = radices[prime_cnt - 1] * (size_t)(exp_needed[prime_cnt - 1] + 1);

    /* target exponent state: all required exponents reached */
    size_t target_state = 0;
    for (int i = 0; i < prime_cnt; ++i)
        target_state += (size_t)exp_needed[i] * radices[i];

    /* 3. DP array: dp[s][state] -> count */
    size_t dp_rows = (n + 1) * total_states;
    seq_t *dp = (seq_t *)calloc(dp_rows, sizeof(seq_t));
    if (!dp) return (seq_t)0;       /* allocation failure fallback */
    dp[0] = (seq_t)1;               /* empty partition */

    uint8_t inc[10];  /* exponents of primes for current part, capped */

    for (idx_t part = 1; part <= n; ++part) {
        /* compute inc vector */
        for (int i = 0; i < prime_cnt; ++i) {
            /* compute exponent of primes[i] in part, capped */
            int e_cap = exp_needed[i];
            int e_here = 0;
            uint64_t x = part;
            while (x % primes[i] == 0) {
                ++e_here;
                if (e_here == e_cap) break;
                x /= primes[i];
            }
            inc[i] = (uint8_t)e_here;
        }

        /* iterate sums descending to ensure each part used at most once */
        for (idx_t sum = n; sum >= part; --sum) {
            size_t pos = sum * total_states;
            size_t prev = (sum - part) * total_states;
            for (size_t state = 0; state < total_states; ++state) {
                seq_t val = dp[prev + state];
                if (!val) continue;
                /* decode current exponents, add inc, cap, and encode back */
                size_t new_state = 0;
                for (int i = 0; i < prime_cnt; ++i) {
                    size_t cur_exp = (state / radices[i]) % (size_t)(exp_needed[i] + 1);
                    size_t new_exp = cur_exp + inc[i];
                    if (new_exp > exp_needed[i]) new_exp = exp_needed[i];
                    new_state += new_exp * radices[i];
                }
                dp[pos + new_state] += val;
            }
        }
    }

    seq_t result = dp[n * total_states + target_state];
    free(dp);
    return result;
}
