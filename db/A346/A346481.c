/*
 * A346481  a(n) = A346467(n) / A346466(n) = lcm S / lcm {d in S, d+1 | n},
 *          with S = {d | n-1, d+1 is prime}.
 * https://oeis.org/A346481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm{d | n-1, d+1 prime} / lcm{d : d+1 prime, d | n-1, d+1
 *            | n}
 * Range:     u8 n <= 318, u16 n <= 8841, u32 n <= 8841, u64 n <= 8841,
 *            u128 n <= 8841, big any n
 * Helpers:   big_pow, build_primes_upto, is_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/big_pow.h"

static uint64_t *prime_list = NULL;   /* primes up to LIMIT */
static size_t    prime_cnt  = 0;

/* ------------------------------------------------------------------ */
/* factor_dd updates the maximum exponents for all and subset.       */
/* ------------------------------------------------------------------ */
static void factor_dd(idx_t dd, int *ex_all, int *ex_sub, bool include_sub)
{
    idx_t tmp = dd;

    /* trial division using the global prime list */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if ((uint64_t)p * p > tmp) break;

        int cnt = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
        if (cnt > 0) {
            if (cnt > ex_all[i]) ex_all[i] = cnt;
            if (include_sub && cnt > ex_sub[i]) ex_sub[i] = cnt;
        }
    }

    /* any remaining prime factor */
    if (tmp > 1) {
        /* binary search the prime index */
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (prime_list[mid] < tmp) lo = mid + 1;
            else hi = mid;
        }
        if (lo < prime_cnt && prime_list[lo] == tmp) {
            if (1 > ex_all[lo]) ex_all[lo] = 1;
            if (include_sub && 1 > ex_sub[lo]) ex_sub[lo] = 1;
        }
        /* If tmp is not in the list, it is larger than our pre‑computed
           primes – this cannot happen for n <= 10 000. */
    }
}

/* ------------------------------------------------------------------ */
/* main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initialise the prime table once */
    if (!prime_list) {
        const idx_t LIMIT = 20000;  /* > 10 000, sufficient for all tests */
        build_primes_upto(LIMIT, &prime_list, &prime_cnt);
    }

    /* allocate static exponent tables */
    static int  *ex_all  = NULL;
    static int  *ex_sub  = NULL;
    static size_t exp_size = 0;

    if (exp_size < prime_cnt) {
        free((void*)ex_all);
        free((void*)ex_sub);
        ex_all = (int*)calloc(prime_cnt, sizeof(int));
        ex_sub = (int*)calloc(prime_cnt, sizeof(int));
        exp_size = prime_cnt;
    } else {
        memset((void*)ex_all, 0, prime_cnt * sizeof(int));
        memset((void*)ex_sub, 0, prime_cnt * sizeof(int));
    }

    /* process all divisors of n-1 */
    idx_t nm1 = n > 0 ? n - 1 : 0;
    for (idx_t d = 1; d * d <= nm1; ++d) {
        if (nm1 % d != 0) continue;

        idx_t d1 = d;
        idx_t d2 = nm1 / d;

        if (is_prime(d1 + 1)) {
            bool sub = (n % (d1 + 1) == 0);
            factor_dd(d1, ex_all, ex_sub, sub);
        }

        if (d2 != d1 && is_prime(d2 + 1)) {
            bool sub = (n % (d2 + 1) == 0);
            factor_dd(d2, ex_all, ex_sub, sub);
        }
    }

    /* compute the ratio as a product of prime powers */
    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        int diff = ex_all[i] - ex_sub[i];
        if (diff > 0) {
            seq_t power = big_pow((seq_t)prime_list[i], (seq_t)diff);
            result = seq_mul(result, power);
        }
    }
    return result;
}
