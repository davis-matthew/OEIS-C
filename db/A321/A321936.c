/*
 * A321936  Number of integer partitions of n containing no 1's, prime powers,
 *          or squarefree numbers.
 * https://oeis.org/A321936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A321936: number of integer partitions of n
   using only allowed parts (composite numbers that are
   neither prime powers nor squarefree). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for DP, SPF and validity tables */
    static seq_t *dp        = NULL;
    static idx_t  dp_size   = 0;
    static uint64_t *spf    = NULL;
    static idx_t  spf_size   = 0;
    static unsigned char *valid = NULL;
    static idx_t  valid_size  = 0;

    /* enlarge tables if needed */
    if (n + 1 > dp_size) {
        /* free old dp */
        #ifdef __cplusplus
        delete[] dp;
        #else
        free(dp);
        #endif
        /* allocate new dp */
        #ifdef __cplusplus
        dp = new seq_t[n + 1]();
        #else
        dp = (seq_t *)calloc(n + 1, sizeof(seq_t));
        #endif
        dp_size = n + 1;
    }
    if (n + 1 > spf_size) {
        #ifdef __cplusplus
        delete[] spf;
        #else
        free(spf);
        #endif
        #ifdef __cplusplus
        spf = new uint64_t[n + 1];
        #else
        spf = (uint64_t *)calloc(n + 1, sizeof(uint64_t));
        #endif
        spf_size = n + 1;
    }
    if (n + 1 > valid_size) {
        #ifdef __cplusplus
        delete[] valid;
        #else
        free(valid);
        #endif
        #ifdef __cplusplus
        valid = new unsigned char[n + 1]();
        #else
        valid = (unsigned char *)calloc(n + 1, sizeof(unsigned char));
        #endif
        valid_size = n + 1;
    }

    /* sieve for smallest prime factor */
    for (idx_t i = 2; i <= n; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            if (i > n / i) continue;   /* i*i would overflow or exceed n */
            for (idx_t j = i * i; j <= n; j += i)
                if (!spf[j]) spf[j] = i;
        }
    }

    /* build validity table: true if number is allowed part */
    for (idx_t m = 2; m <= n; ++m) {
        unsigned char distinct = 0;      /* number of distinct primes */
        unsigned char has_square = 0;    /* at least one exponent > 1 */
        idx_t x = m;
        while (x > 1) {
            uint64_t p = spf[x];
            unsigned char cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            ++distinct;
            if (cnt > 1) has_square = 1;
        }
        /* allowed iff distinct >= 2 and at least one repeated exponent */
        valid[m] = (distinct >= 2 && has_square);
    }

    /* DP for partitions: dp[i] = number of partitions of i */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;
    for (idx_t part = 2; part <= n; ++part) {
        if (!valid[part]) continue;
        for (idx_t i = part; i <= n; ++i)
            dp[i] = dp[i] + dp[i - part];
    }
    return dp[n];
}
