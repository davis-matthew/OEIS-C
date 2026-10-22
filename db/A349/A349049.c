/*
 * A349049  Number of prime factors (with multiplicity) of the denominator of
 *          the harmonic number H(n) = Sum_{k=1..n} 1/k.
 * https://oeis.org/A349049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(A002805(n)) a(n) is the number of prime factors
 *            (counted with multiplicity) of the denominator of the harmonic
 *            number H(n) = Sum_{k=1..n} 1/k
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses only arithmetic on integers that are at most
   2^30, so it works safely for any seq_t size (uint8/16/32/64/128 or
   arbitrary‑precision).  For each prime p<=n it maintains the
   numerator and denominator modulo p^(2·e(p)), where e(p)=⌊log_p n⌋.
   The exponent of p in the reduced denominator is accumulated
   incrementally; the answer is the sum of these exponents. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- Sieve utilities ---------- */
#define MAX_N    10000
#define MAX_PR  1300      /* >= number of primes <= 10000 */

static int spf[MAX_N + 1];
static int primes[MAX_PR];
static int primes_cnt = 0;
static int sieve_limit = 0;

/* initialise the sieve up to `limit` (inclusive) */
static void init_sieve(int limit) {
    if (limit <= sieve_limit) return;
    for (int i = 2; i <= limit; ++i) spf[i] = i;
    for (int i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {
            for (int j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    primes_cnt = 0;
    for (int i = 2; i <= limit; ++i)
        if (spf[i] == i) primes[primes_cnt++] = i;
    sieve_limit = limit;
}

/* ---------- Main function ---------- */
seq_t a(idx_t n) {
    if (n < 1U) return 0;

    /* Limit to the maximum value we handle */
    int N = (int)n;
    if (N > MAX_N) N = MAX_N;          /* safety, should not happen */

    init_sieve(N);

    /* per‑prime data */
    uint32_t exp_den[MAX_PR];
    uint64_t num_mod[MAX_PR];
    uint64_t den_mod[MAX_PR];
    uint64_t mod_base[MAX_PR];          /* p^(maxExp)  */
    int max_exp[MAX_PR];
    uint64_t powp[MAX_PR][35];          /* powp[j][e] = p^e, 0<=e<=max_exp */

    /* precompute per‑prime tables */
    for (int i = 0; i < primes_cnt; ++i) {
        int p = primes[i];
        /* e(p) = floor(log_p N)  ==>  largest exponent with p^e <= N */
        int e = 0;
        uint64_t m = 1;
        while (m <= (uint64_t)N) {
            ++e;
            m *= (uint64_t)p;
        }
        max_exp[i] = e * 2;              /* safe upper bound on exponent needed */
        uint64_t pow = 1;
        powp[i][0] = 1;
        for (int j = 1; j <= max_exp[i]; ++j) {
            pow *= (uint64_t)p;
            powp[i][j] = pow;
        }
        mod_base[i] = powp[i][max_exp[i]];
        exp_den[i] = 0;
        num_mod[i] = 0;
        den_mod[i] = 1 % mod_base[i];
    }

    /* temporary space for factorisation of a single k */
    int fprimes[16];
    int fexps[16];
    int fcnt;

    /* main loop over k = 1 .. N */
    for (uint64_t k = 1; k <= (uint64_t)N; ++k) {
        /* factorise k */
        fcnt = 0;
        uint64_t kk = k;
        while (kk > 1) {
            int fp = spf[(int)kk];
            int cnt = 0;
            while (kk % fp == 0) {
                kk /= fp;
                ++cnt;
            }
            fprimes[fcnt] = fp;
            fexps[fcnt] = cnt;
            ++fcnt;
        }

        /* update all primes */
        for (int i = 0; i < primes_cnt; ++i) {
            int p = primes[i];
            uint64_t M = mod_base[i];
            uint64_t kmod = k % M;

            uint64_t new_num = (num_mod[i] * kmod + den_mod[i]) % M;
            uint64_t new_den = (den_mod[i] * kmod) % M;

            /* exponent of p in k */
            int vp = 0;
            for (int t = 0; t < fcnt; ++t) {
                if (fprimes[t] == p) { vp = fexps[t]; break; }
            }

            int new_exp_den = (int)exp_den[i] + vp;

            /* compute exponent of p in new_num */
            int v_num = 0;
            uint64_t temp = new_num;
            while (v_num < new_exp_den && temp % (uint64_t)p == 0) {
                temp /= (uint64_t)p;
                ++v_num;
            }

            int gcd_exp = (v_num < new_exp_den) ? v_num : new_exp_den;

            /* divide numerator and denominator by p^gcd_exp */
            uint64_t div = powp[i][gcd_exp];
            num_mod[i] = new_num / div;
            den_mod[i] = new_den / div;
            exp_den[i] = (uint32_t)(new_exp_den - gcd_exp);
        }
    }

    /* accumulate the answer */
    uint64_t res = 0;
    for (int i = 0; i < primes_cnt; ++i)
        res += exp_den[i];

    return (seq_t)res;
}
