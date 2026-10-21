/*
 * A268045  Least number k > 1 such that C(n+k,n) is squarefree.
 * https://oeis.org/A268045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 214, u32 n <= 477, u64 n <= 477,
 *            u128 n <= 477, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = least k > 1 such that C(n+k,n) is squarefree. */
/* Uses incremental prime‑factor updates to test squarefreeness without
   constructing the whole binomial. */

#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#define MAX_SPF 2000000          /* upper bound for precomputed primes */

static uint32_t spf[MAX_SPF + 1];   /* smallest prime factor */
static int      exp_cnt[MAX_SPF + 1]; /* current exponent count for each prime */
static bool init_done = false;

/* linear sieve for smallest prime factors */
static void init_sieve(void)
{
    if (init_done) return;
    for (size_t i = 0; i <= MAX_SPF; ++i) spf[i] = 0;
    for (size_t i = 2; i <= MAX_SPF; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = (uint32_t)i;
            if (i <= MAX_SPF / i)
                for (size_t j = i * i; j <= MAX_SPF; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    init_done = true;
}

/* Update exponent counts for factorization of x with sign (+1 or -1),
   adjusting bad_count (number of primes with exponent > 1). */
static void factor_update(idx_t x, int sign, int *exp_cnt, int *bad_count)
{
    while (x > 1) {
        uint32_t p = spf[(size_t)x];
        int cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
        int before = exp_cnt[p];
        int after = before + sign * cnt;
        exp_cnt[p] = after;
        if (before <= 1 && after > 1) ++(*bad_count);
        else if (before > 1 && after <= 1) --(*bad_count);
    }
}

seq_t a(idx_t n)
{
    init_sieve();
    memset(exp_cnt, 0, sizeof(exp_cnt));
    int bad_count = 0;

    /* Initial product for k = 1: C(n+1,n) = n+1 */
    factor_update(n + 1, +1, exp_cnt, &bad_count);
    /* Denominator 1 is skipped */

    for (idx_t k = 2; ; ++k) {
        factor_update(n + k, +1, exp_cnt, &bad_count);
        factor_update(k, -1, exp_cnt, &bad_count);
        if (bad_count == 0) return (seq_t)k;   /* squarefree found */
    }
}
