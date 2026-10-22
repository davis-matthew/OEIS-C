/*
 * A324507  a(n) = denominator of Product_{d|n} (d/tau(d)) where tau(k) = the
 *          number of divisors of k (A000005).
 * https://oeis.org/A324507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Product_{d|n} (d / tau(d))
 * Range:     u8 n <= 29, u16 n <= 209, u32 n <= 629, u64 n <= 2309,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use only standard integer types and the utilities provided by the
   framework.  The function computes the denominator of the fraction
   N/D where

        N = ∏_{d|n} d            = n^{τ(n)/2}
        D = ∏_{d|n} τ(d)

   The result equals D / gcd(N,D).  The implementation uses divisor
   enumeration (which is very cheap because n ≤ 10 000 in the test
   suite) and elementary prime factorisation of τ(d), which is always
   < 300.  The code is fully portable to C11 and C++17. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utilities for arbitrary‑precision arithmetic */
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Small primes up to 257 – enough for all τ(d) that can appear
   when n ≤ 10 000 (τ(n) ≤ 120).  The list contains all primes ≤ 257. */
static const uint32_t SMALL_PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
    233, 239, 241, 251, 257
};
#define NUM_SMALL_PRIMES (sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]))

/* --------------------------------------------------------------------- */
/* helpers for dynamic small maps (prime → exponent)                     */
/* --------------------------------------------------------------------- */
typedef struct { uint64_t p; uint64_t e; } Exp;

static void add_exp(Exp *arr, int *cnt, uint64_t p, uint64_t e)
{
    for (int i = 0; i < *cnt; i++) {
        if (arr[i].p == p) {
            arr[i].e += e;
            return;
        }
    }
    arr[*cnt].p = p;
    arr[*cnt].e = e;
    (*cnt)++;
}

/* --------------------------------------------------------------------- */
/* recursive generation of all divisors of n (by exponents) and
   accumulation of the prime exponents of τ(d).
   The arguments max_exp[i] hold the maximum exponent of prime i in n.   */
/* --------------------------------------------------------------------- */
static void dfs_divisors(int idx, int nf, const int *max_exp,
                         int *cur_exp, Exp *d_arr, int *d_cnt)
{
    if (idx == nf) {
        /* compute τ(d) for the current divisor */
        int tau = 1;
        for (int i = 0; i < nf; i++)
            tau *= (cur_exp[i] + 1);

        if (tau <= 1)
            return;                 /* τ(d) = 1 contributes nothing */

        /* factor τ(d) and add to d_arr */
        int x = tau;
        for (size_t j = 0; j < NUM_SMALL_PRIMES && SMALL_PRIMES[j] * SMALL_PRIMES[j] <= x; j++) {
            int p = SMALL_PRIMES[j];
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                cnt++;
            }
            if (cnt)
                add_exp(d_arr, d_cnt, (uint64_t)p, (uint64_t)cnt);
        }
        if (x > 1)
            add_exp(d_arr, d_cnt, (uint64_t)x, 1ULL);

        return;
    }

    for (int e = 0; e <= max_exp[idx]; ++e) {
        cur_exp[idx] = e;
        dfs_divisors(idx + 1, nf, max_exp, cur_exp, d_arr, d_cnt);
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* factor n (trial division – enough for n ≤ 10 000)                  */
    /* ------------------------------------------------------------------ */
    uint64_t nn = (uint64_t)n;
    uint64_t fact_pr[64];
    int          exp_pr[64];
    int          nf = 0;  /* number of distinct prime factors */

    for (uint64_t d = 2; d * d <= nn; ++d) {
        if (nn % d == 0) {
            int cnt = 0;
            while (nn % d == 0) {
                nn /= d;
                cnt++;
            }
            if (nf < 64) {
                fact_pr[nf] = d;
                exp_pr[nf]  = cnt;
                nf++;
            }
        }
    }
    if (nn > 1) {
        if (nf < 64) {
            fact_pr[nf] = nn;
            exp_pr[nf]  = 1;
            nf++;
        }
    }

    /* ------------------------------------------------------------------ */
    /* tau(n) and the exponents of the numerator N = n^{τ(n)/2}            */
    /* ------------------------------------------------------------------ */
    idx_t tau_n = 1;
    for (int i = 0; i < nf; ++i)
        tau_n *= (idx_t)(exp_pr[i] + 1);

    /* store numerator exponents (prime → exponent) */
    Exp num_exp[64];
    int num_cnt = 0;
    for (int i = 0; i < nf; ++i) {
        idx_t exp_n = (idx_t)exp_pr[i] * tau_n / 2;  /* integer division */
        add_exp(num_exp, &num_cnt, fact_pr[i], exp_n);
    }

    /* ------------------------------------------------------------------ */
    /* enumerate all divisors of n and accumulate the exponents of D = ∏ τ(d) */
    /* ------------------------------------------------------------------ */
    int  max_exp[64];
    int  cur_exp[64];
    for (int i = 0; i < nf; ++i)
        max_exp[i] = exp_pr[i];

    Exp den_exp[128];
    int den_cnt = 0;

    dfs_divisors(0, nf, max_exp, cur_exp, den_exp, &den_cnt);

    /* ------------------------------------------------------------------ */
    /* compute the denominator: D / gcd(N,D)                              */
    /* ------------------------------------------------------------------ */
    seq_t result = (seq_t)1;

    for (int i = 0; i < den_cnt; ++i) {
        uint64_t prime = den_exp[i].p;
        uint64_t d_exp = den_exp[i].e;

        /* find exponent of the same prime in N (if any) */
        uint64_t n_exp = 0;
        for (int j = 0; j < num_cnt; ++j)
            if (num_exp[j].p == prime) {
                n_exp = num_exp[j].e;
                break;
            }

        uint64_t common = (d_exp < n_exp) ? d_exp : n_exp;
        uint64_t rem_exp = d_exp - common;

        if (rem_exp > 0) {
            seq_t term = seq_pow((seq_t)prime, (idx_t)rem_exp);
            result = seq_mul(result, term);
        }
    }

    return result;
}
