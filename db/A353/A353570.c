/*
 * A353570  a(n) = 1 if A003963(A122111(n)) is equal to A003963(n), and 0
 *          otherwise.
 * https://oeis.org/A353570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A003963(n) = A329382(n), otherwise 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 141 terms through n = 9579 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* ---------- A003963 ---------- */
static bool A003963_is_prime(uint64_t p) {
    if (p < 2) return false;
    if (p == 2) return true;
    if (p % 2 == 0) return false;
    if (p == 3) return true;
    if (p % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= p; i += 6) {
        if (p % i == 0 || p % (i + 2) == 0) return false;
    }
    return true;
}

static uint64_t A003963_count_primes(uint64_t p) {
    if (p < 2) return 0;
    uint64_t count = 1;
    if (p == 2) return count;
    for (uint64_t i = 3; i <= p; i += 2) {
        if (A003963_is_prime(i)) count++;
    }
    return count;
}

static seq_t A003963_pow_seq(seq_t base, uint64_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

static seq_t A003963_a(idx_t n) {
    if (n == 1) return 1;
    uint64_t m = (uint64_t)n;
    seq_t result = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                exp++;
                m /= p;
            }
            uint64_t k = A003963_count_primes(p);
            result *= A003963_pow_seq((seq_t)k, exp);
        }
    }
    if (m > 1) {
        uint64_t k = A003963_count_primes(m);
        result *= (seq_t)k;
    }
    return result;
}

/* ---------- A329382 ---------- */
static uint64_t *A329382_prime_list = NULL;
static size_t    A329382_prime_count = 0;
static size_t    A329382_prime_cap = 0;
static idx_t     A329382_prime_sieve_limit = 0;

static void A329382_ensure_primes_upto(idx_t limit)
{
    if (limit <= A329382_prime_sieve_limit)
        return;

    bool *isp = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    for (size_t i = 2; i <= (size_t)limit; ++i)
        isp[i] = true;

    for (size_t i = 2; (idx_t)i * (idx_t)i <= limit; ++i)
        if (isp[i])
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                isp[j] = false;

    size_t new_primes = 0;
    for (size_t i = (size_t)A329382_prime_sieve_limit + 1; i <= (size_t)limit; ++i)
        if (isp[i]) ++new_primes;

    if (A329382_prime_cap < A329382_prime_count + new_primes) {
        size_t new_cap = A329382_prime_count + new_primes;
        uint64_t *tmp = (uint64_t *)realloc(A329382_prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        A329382_prime_list = tmp;
        A329382_prime_cap = new_cap;
    }

    for (size_t i = (size_t)A329382_prime_sieve_limit + 1; i <= (size_t)limit; ++i)
        if (isp[i])
            A329382_prime_list[A329382_prime_count++] = (uint64_t)i;

    free(isp);
    A329382_prime_sieve_limit = limit;
}

static idx_t A329382_prime_index(uint64_t p)
{
    A329382_ensure_primes_upto((idx_t)p);
    size_t lo = 0, hi = A329382_prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (A329382_prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (idx_t)(lo + 1);
}

static seq_t A329382_seq_pow(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

static seq_t A329382_a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t fac_primes[32];
    uint64_t fac_exps[32];
    size_t   fac_cnt = 0;
    factor_u64((uint64_t)n, fac_primes, fac_exps, &fac_cnt);

    idx_t indices[32];
    for (size_t i = 0; i < fac_cnt; ++i)
        indices[i] = A329382_prime_index(fac_primes[i]);

    for (size_t i = 0; i < fac_cnt; ++i)
        for (size_t j = i + 1; j < fac_cnt; ++j)
            if (indices[i] < indices[j]) {
                idx_t ti = indices[i];
                indices[i] = indices[j];
                indices[j] = ti;

                uint64_t te = fac_exps[i];
                fac_exps[i] = fac_exps[j];
                fac_exps[j] = te;
            }

    seq_t ans = (seq_t)1;
    seq_t cum = (seq_t)0;
    for (size_t pos = 0; pos < fac_cnt; ++pos) {
        cum += (seq_t)fac_exps[pos];
        idx_t next_k = (pos + 1 < fac_cnt) ? indices[pos + 1] : 0;
        idx_t diff   = indices[pos] - next_k;
        ans *= A329382_seq_pow(cum, diff);
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t x = A003963_a(n);
    seq_t y = A329382_a(n);
    return (x == y) ? (seq_t)1 : (seq_t)0;
}
