/*
 * A324301  a(n) = [x^(n^2)] Sum_{m>=0} x^m * (x^(m+1) + i)^m / (1 +
 *          i*x^(m+1))^(m+1), for n >= 0.
 * https://oeis.org/A324301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n even → sum_{q|n²+1} S(q−1, (n²+1)/q−1) , where S(t,k) =
 *            Σ_{u=0}^{min(t,k)} binom(t,u)*binom(t+k−u,k−u)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 29, u128 n <= 67,
 *            big any n
 * Helpers:   binomial_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* All odd n give a(n)=0. a(0)=1. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial_sum.h"

/* ------------- Prime sieve (static) ------------- */
#define MAX_PRIME 10001
static int prime_list[2000];
static int prime_count = 0;

static void init_primes(void) {
    if (prime_count != 0) return;          /* already initialized */
    char is_prime[MAX_PRIME];
    for (int i = 2; i < MAX_PRIME; ++i) is_prime[i] = 1;
    for (int i = 2; i * i < MAX_PRIME; ++i)
        if (is_prime[i])
            for (int j = i * i; j < MAX_PRIME; j += i)
                is_prime[j] = 0;
    for (int i = 2; i < MAX_PRIME; ++i)
        if (is_prime[i] && prime_count < 2000)
            prime_list[prime_count++] = i;
}

/* ------------- Factorisation of uint64_t ---------------- */
static void factor_uint64(uint64_t n,
                          uint64_t pf[],  int exp[],
                          int *cnt) {
    int local_cnt = 0;
    uint64_t tmp = n;
    for (int i = 0; i < prime_count && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= tmp; ++i) {
        int p = prime_list[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pf[local_cnt] = (uint64_t)p;
            exp[local_cnt] = e;
            ++local_cnt;
        }
    }
    if (tmp > 1) {
        pf[local_cnt] = tmp;
        exp[local_cnt] = 1;
        ++local_cnt;
    }
    *cnt = local_cnt;
}

/* ------------- Divisor generation ----------------- */
static void gen_divisors_recursive(int idx, uint64_t curr,
                                   const uint64_t pf[], const int exp[],
                                   int fac_cnt,
                                   uint64_t *divs, size_t *divs_cnt) {
    if (idx == fac_cnt) {
        divs[*divs_cnt] = curr;
        ++(*divs_cnt);
        return;
    }
    uint64_t p = pf[idx];
    int e = exp[idx];
    uint64_t mult = 1;
    for (int i = 0; i <= e; ++i) {
        gen_divisors_recursive(idx + 1, curr * mult,
                               pf, exp,
                               fac_cnt,
                               divs, divs_cnt);
        mult *= p;
    }
}

/* ------------- Main sequence function ---------------- */
seq_t a(idx_t n) {
    init_primes();

    /* n=0 */
    if (n == 0) return 1;

    /* odd n → 0 */
    if (n & 1) return 0;

    /* n even */
    uint64_t n64 = (uint64_t)n;
    uint64_t m = n64 * n64 + 1ULL;          /* m = n^2 + 1 (always odd) */

    /* factor m */
    uint64_t pf[16];
    int exp[16];
    int fac_cnt;
    factor_uint64(m, pf, exp, &fac_cnt);

    /* generate all divisors of m */
    uint64_t divs[2000];
    size_t divs_cnt = 0;
    gen_divisors_recursive(0, 1ULL, pf, exp, fac_cnt, divs, &divs_cnt);

    /* accumulate result */
    seq_t result = 0;
    for (size_t i = 0; i < divs_cnt; ++i) {
        uint64_t qll = divs[i];
        seq_t q = (seq_t)qll;
        seq_t t = q - 1;
        seq_t d = (seq_t)(m / qll);
        seq_t k = d - 1;
        result += binomial_sum(t, k);
    }
    return result;
}
