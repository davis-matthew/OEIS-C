/*
 * A245661  Number of p*pp-divisors of n where a p*pp-number is the product of
 *          a prime and perfect power (see A245303).
 * https://oeis.org/A245661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count divisors d of n such that d can be written as p * pp where
 *            p is a prime and pp is a perfect power (1 or a^b with
 *            a>=2,b>=2). a(n) = number of such divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* helper: check if x is a perfect power (1 or a^b with a>=2,b>=2) */
static bool is_perfect_power_uint64(uint64_t x) {
    if (x <= 1) return true;               /* 1 is considered a perfect power */
    /* try all possible bases a starting at 2 */
    uint64_t a = 2;
    for (; a * a <= x; ++a) {
        uint64_t power = a * a;            /* a^2 */
        while (power < x) {
            if (power > x / a) break;     /* overflow guard */
            power *= a;                   /* a^k */
        }
        if (power == x) return true;
    }
    return false;
}

/* factorize n into primes[] and exps[]; returns number of prime factors */
static int factorize_uint64(uint64_t n, uint64_t primes_arr[], int exps_arr[]) {
    int cnt = 0;
    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            int e = 0;
            do {
                n /= p;
                ++e;
            } while (n % p == 0);
            primes_arr[cnt] = p;
            exps_arr[cnt] = e;
            ++cnt;
        }
    }
    if (n > 1) {           /* remaining prime factor */
        primes_arr[cnt] = n;
        exps_arr[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* generate all divisors of n given its prime factorization */
static void generate_divisors(const uint64_t primes_arr[], const int exps_arr[], int pf_cnt,
                              uint64_t divs[], int *div_cnt) {
    *div_cnt = 1;
    divs[0] = 1;
    for (int i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes_arr[i];
        int e = exps_arr[i];
        int old_cnt = *div_cnt;
        uint64_t p_pow = 1;
        for (int k = 1; k <= e; ++k) {
            p_pow *= p;               /* p^k */
            for (int j = 0; j < old_cnt; ++j) {
                divs[*div_cnt] = divs[j] * p_pow;
                ++(*div_cnt);
            }
        }
    }
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined below offset, but harmless */
    uint64_t N = (uint64_t)n;

    /* factorize N */
    uint64_t primes[64];
    int exps[64];
    int pf_cnt = factorize_uint64(N, primes, exps);

    /* generate all divisors of N */
    uint64_t divs[256];
    int div_cnt;
    generate_divisors(primes, exps, pf_cnt, divs, &div_cnt);

    /* count divisors that are p*pp */
    int count = 0;
    for (int i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        /* skip 1, because 1 cannot be written as p*pp with p prime */
        if (d == 1) continue;

        /* factor d to find the prime factor p */
        uint64_t dp = d;
        bool good = false;
        for (uint64_t p = 2; p <= dp / p && !good; ++p) {
            if (dp % p == 0) {
                uint64_t pp = d / p;
                if (is_perfect_power_uint64(pp)) {
                    good = true;
                    break;
                }
                while (dp % p == 0) dp /= p;  /* remove all instances of p */
            }
        }
        if (!good && dp > 1) {   /* remaining prime factor */
            uint64_t p = dp;
            uint64_t pp = d / p;
            if (is_perfect_power_uint64(pp)) good = true;
        }
        if (good) ++count;
    }

    return (seq_t)count;
}
