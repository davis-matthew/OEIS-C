/*
 * A326515  Number of factorizations of n into factors > 1 where every factor
 *          has the same average of prime indices.
 * https://oeis.org/A326515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of factorizations of n into factors > 1 where
 *            every factor has the same average of prime indices. The average
 *            of the prime indices of a factor f is (∑prime_index(p) *
 *            exponent_of_p_in_f) / (∑exponent_of_p_in_f). For a factorization
 *            to satisfy the condition each factor must have this...
 * Range:     u8 n <= 9598, u16 n <= 9598, u32 n <= 9598, u64 n <= 9598,
 *            u128 n <= 9598, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Global data for the small sieve */
#define MAXN 200000          /* sufficient for all expected n (≤ 10000) */
static uint32_t spf[MAXN + 1];
static uint16_t prime_idx[MAXN + 1];
static uint32_t primes_arr[20000];
static size_t prime_cnt = 0;
static int init_sieve_done = 0;

/* Build smallest prime factor array and prime indices */
static void init_sieve(void) {
    if (init_sieve_done) return;
    for (uint32_t i = 0; i <= MAXN; ++i) spf[i] = i;
    for (uint32_t i = 2; i * i <= MAXN; ++i) {
        if (spf[i] == i) {
            for (uint32_t j = i * i; j <= MAXN; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    size_t idx = 0;
    for (uint32_t i = 2; i <= MAXN; ++i) {
        if (spf[i] == i) {
            primes_arr[idx++] = i;
            prime_idx[i] = (uint16_t)(idx);   /* 1‑based index */
        }
    }
    prime_cnt = idx;
    init_sieve_done = 1;
}

/* Helper: factorize n using the global spf array.
   The result is written into arrays 'p' and 'e', and count is stored in 'cnt'.
   The arrays must be large enough (at most 20 elements for n ≤ 2⁶⁴). */
static void factor_uint64(uint64_t n, uint32_t *p, uint8_t *e, size_t *cnt) {
    size_t pos = 0;
    while (n > 1) {
        uint32_t prime = spf[n];
        uint8_t exp = 0;
        while (n % prime == 0) {
            n /= prime;
            ++exp;
        }
        p[pos] = prime;
        e[pos] = exp;
        ++pos;
    }
    *cnt = pos;
}

/* Recursive enumeration of unordered factorizations of n. */
static void dfs_factor(uint64_t rem, uint64_t min_factor,
                       uint64_t S_target, uint64_t T_target,
                       seq_t *cnt) {
    for (uint64_t f = min_factor; f <= rem / 2; ++f) {
        if (rem % f == 0) {
            uint64_t t = f;
            uint64_t S_f = 0, T_f = 0;
            while (t > 1) {
                uint32_t p = spf[t];
                uint8_t e = 0;
                while (t % p == 0) {
                    t /= p;
                    ++e;
                }
                S_f += (uint64_t)e * (uint64_t)prime_idx[p];
                T_f += e;
            }
            if (S_f * T_target == S_target * T_f)
                dfs_factor(rem / f, f, S_target, T_target, cnt);
        }
    }

    if (rem >= min_factor) {
        uint64_t t = rem;
        uint64_t S_f = 0, T_f = 0;
        while (t > 1) {
            uint32_t p = spf[t];
            uint8_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            S_f += (uint64_t)e * (uint64_t)prime_idx[p];
            T_f += e;
        }
        if (S_f * T_target == S_target * T_f)
            (*cnt)++;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 1) return 1;
    init_sieve();

    uint32_t p[20];
    uint8_t e[20];
    size_t pcnt;
    factor_uint64((uint64_t)n, p, e, &pcnt);

    uint64_t S = 0, T = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        S += (uint64_t)e[i] * (uint64_t)prime_idx[p[i]];
        T += e[i];
    }

    seq_t cnt = 0;
    dfs_factor((uint64_t)n, 2, S, T, &cnt);
    return cnt;
}
