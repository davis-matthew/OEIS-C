/*
 * A378449  Characteristic function of A083209: a(n) = 1 if there is exactly
 *          one subset of the divisors of n such that the complement has the
 *          same sum, otherwise 0.
 * https://oeis.org/A378449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=1 iff there is exactly one partition of the divisors of n
 *            into two complementary subsets of equal sum; otherwise 0
 * Range:     u8 n <= 4399, u16 n <= 9790, u32 n <= 9790, u64 n <= 9790,
 *            u128 n <= 9790, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* factorization by trial division – sufficient for n <= 10000 */
static void factorize_u64(uint64_t n, uint64_t *pr, uint8_t *ex, size_t *cnt)
{
    size_t i = 0;
    if (n % 2 == 0) {
        uint8_t e = 0;
        while (n % 2 == 0) { n /= 2; ++e; }
        pr[i] = 2; ex[i] = e; ++i;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint8_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            pr[i] = p; ex[i] = e; ++i;
        }
    }
    if (n > 1) {
        pr[i] = n; ex[i] = 1; ++i;
    }
    *cnt = i;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset ensures n>=1, but guard anyway */

    /* --------------------------------------------------------------
       Step 1: factorize n and generate all its divisors
       -------------------------------------------------------------- */
    uint64_t primes[16];
    uint8_t   exps[16];
    size_t    pfcnt = 0;
    factorize_u64((uint64_t)n, primes, exps, &pfcnt);

    /* array to hold divisors – 128 is sufficient for n <= 10000 */
    seq_t divs[128];
    size_t divs_cnt = 1;
    divs[0] = (seq_t)1;

    for (size_t pi = 0; pi < pfcnt; ++pi) {
        seq_t p = (seq_t)primes[pi];
        size_t old_cnt = divs_cnt;
        seq_t pow = 1;
        for (uint8_t e = 1; e <= exps[pi]; ++e) {
            pow *= p;                     /* p^e */
            for (size_t j = 0; j < old_cnt; ++j)
                divs[divs_cnt++] = divs[j] * pow;
        }
    }

    /* --------------------------------------------------------------
       Step 2: total sum of divisors; if odd, no partition
       -------------------------------------------------------------- */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < divs_cnt; ++i) sum += divs[i];

    if (sum & (seq_t)1) return (seq_t)0;  /* odd sum */

    seq_t half = sum >> 1;                /* sum / 2 */

    /* target must fit into size_t for the DP array size */
    if (half > (seq_t)SIZE_MAX) return (seq_t)0;
    size_t target = (size_t)half;

    /* --------------------------------------------------------------
       Step 3: subset sum DP counting subsets that sum to 'half', 
                capped at 3 so we can detect >2
       -------------------------------------------------------------- */
    unsigned char *dp = (unsigned char *)calloc(target + 1, sizeof(unsigned char));
    if (!dp) return (seq_t)0;            /* allocation failure → safe default 0 */

    dp[0] = 1;                           /* empty subset */

    for (size_t i = 0; i < divs_cnt; ++i) {
        seq_t d = divs[i];
        if (d > half) continue;          /* skip too large divisors */
        size_t sd = (size_t)d;
        for (size_t s = target; s >= sd; --s) {
            unsigned char prev = dp[s - sd];
            if (!prev) continue;
            unsigned int new_val = dp[s] + prev;
            if (new_val > 3) new_val = 3; /* cap at 3 to record >2 */
            dp[s] = (unsigned char)new_val;
            if (s == sd) break;           /* avoid unsigned wrap on subtraction */
        }
    }

    unsigned char ways = dp[target];
    free(dp);

    return (seq_t)(ways == 2 ? 1 : 0);
}
