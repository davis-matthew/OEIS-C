/*
 * A378857  a(n) is the smallest k > 1 such that n^k starts and ends with n,
 *          or -1 if there is no such k.
 * https://oeis.org/A378857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = smallest k>1 such that n^k starts and ends with n, or -1
 *            if none
 * Range:     i8 n <= 8, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   digits_uint64, gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation follows the algorithm described in the OEIS entry:
 *  1. Let d be the number of decimal digits of n and M = 10^d.
 *  2. The condition that n^k ends with n is equivalent to
 *     n^(k-1) ≡ 1 (mod b) with  b = M / gcd(n, M).
 *  3. Let t be the multiplicative order of n modulo b.
 *     Then all solutions of the trailing condition are k = 1 + t*m,  m≥1.
 *  4. For each such k we test whether the leading digits of n^k equal n
 *     using the logarithmic formula:
 *        frac = k*log10(n) (fractional part)
 *        leading = floor(10^(frac + d-1))
 *     If leading == n we return k.  Otherwise we increase m.
 *  5. If no k is found the function returns -1.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/digits_uint64.h"
#include "../utils/gcd_uint64.h"

/* ---------- constants & global tables ---------- */
#define MAX_PRIME_LIMIT 100000
static uint32_t primes[MAX_PRIME_LIMIT / 9];
static size_t primes_cnt = 0;

static void init_primes(void) {
    static bool done = false;
    if (done) return;
    bool *is_prime = (bool*)malloc(MAX_PRIME_LIMIT + 1);
    if (!is_prime) exit(1); /* allocation failure */
    memset(is_prime, 1, MAX_PRIME_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= MAX_PRIME_LIMIT; ++i)
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                is_prime[j] = 0;
        }
    for (uint32_t i = 2; i <= MAX_PRIME_LIMIT; ++i)
        if (is_prime[i]) primes[primes_cnt++] = i;
    free(is_prime);
    done = true;
}

/* ---------- gcd ---------- */

/* ---------- factorisation into primes (unsigned 64-bit) ---------- */
static void factor_uint64(uint64_t n, uint64_t primes_arr[], int exps[], int *cnt) {
    int c = 0;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if (n % p == 0) {
            int e = 0;
            do {
                n /= p;
                ++e;
            } while (n % p == 0);
            primes_arr[c] = p;
            exps[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        primes_arr[c] = n;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* ---------- Euler's totient phi for 64‑bit n ---------- */
static uint64_t phi_uint64(uint64_t n) {
    uint64_t result = n;
    uint64_t primes_arr[20];
    int exps[20];
    int cnt, i;
    factor_uint64(n, primes_arr, exps, &cnt);
    for (i = 0; i < cnt; ++i)
        result = result / primes_arr[i] * (primes_arr[i] - 1);
    return result;
}

/* ---------- modular exponentiation (uint64_t) ---------- */
static uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = (__uint128_t)res * base % mod;
        base = (__uint128_t)base * base % mod;
        exp >>= 1;
    }
    return res;
}

/* ---------- generate all divisors of num ---------- */
static void generate_divisors_rec(int idx, uint64_t cur,
                                  uint64_t primes_arr[], int exps[], int cnt,
                                  uint64_t *divs, int *div_cnt) {
    if (idx == cnt) {
        divs[(*div_cnt)++] = cur;
        return;
    }
    for (int e = 0; e <= exps[idx]; ++e) {
        generate_divisors_rec(idx + 1, cur, primes_arr, exps, cnt, divs, div_cnt);
        cur *= primes_arr[idx];
    }
}

static void generate_divisors(uint64_t num, uint64_t *divs, int *div_cnt) {
    uint64_t primes_arr[20];
    int exps[20];
    int cnt, i;
    factor_uint64(num, primes_arr, exps, &cnt);
    *div_cnt = 0;
    generate_divisors_rec(0, 1, primes_arr, exps, cnt, divs, div_cnt);
}

/* ---------- number of decimal digits ---------- */

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;

    init_primes();

    uint64_t N = (uint64_t)n;
    unsigned d = digits_uint64(N);

    /* compute 10^d */
    uint64_t M = 1;
    for (unsigned i = 0; i < d; ++i) M *= 10ULL;

    uint64_t g = gcd_uint64(N, M);
    uint64_t b = M / g;                /* modulus for trailing condition */
    if (b == 0) return (seq_t)-1;      /* should not happen */

    /* If N mod b == 0 then trailing condition impossible */
    if (N % b == 0) return (seq_t)-1;

    /* find multiplicative order of N modulo b */
    uint64_t phi_b = phi_uint64(b);
    uint64_t divs[256];
    int div_cnt = 0;
    generate_divisors(phi_b, divs, &div_cnt);

    /* sort divisors ascending */
    for (int i = 0; i < div_cnt - 1; ++i)
        for (int j = i + 1; j < div_cnt; ++j)
            if (divs[i] > divs[j]) {
                uint64_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }

    uint64_t order = 0;
    for (int i = 0; i < div_cnt; ++i) {
        if (modpow_uint64(N % b, divs[i], b) == 1) {
            order = divs[i];
            break;
        }
    }
    if (order == 0) return (seq_t)-1;   /* no order -> impossible */

    /* first candidate k = order + 1 */
    uint64_t step = order;
    long double log10_n = log10l((long double)N);

    for (uint64_t k = order + 1;; k += step) {
        /* leading digit check via logarithm */
        long double x = (long double)k * log10_n;
        long double frac = x - floorl(x);
        long double leading_val = powl(10.0L, frac + (long double)d - 1.0L);
        uint64_t leading = (uint64_t)(leading_val + 1e-12L);
        if (leading == N)
            return (seq_t)k;

        /* safety guard: if k grows too large, give up */
        if (k - (order + 1) > 2000000ULL) break;
    }
    return (seq_t)-1;
}
