/*
 * A363259  Expansion of Sum_{k>0} k * x^(2*k) / (1 - x^(4*k-1)).
 * https://oeis.org/A363259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4) * ∑_{d | (4*n-2), d≡3 (mod 4)} (d+1)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes.h"

/* static cache for primes up to a fixed limit */
static idx_t *g_primes = NULL;
static size_t g_prime_cnt = 0;
static void ensure_primes(void) {
    if (!g_primes) {
        /* 200 000 is ample for n up to 10 000; ensures all needed primes are present. */
        idx_t limit = 200000;
        build_primes(limit, &g_primes, &g_prime_cnt);
    }
}

/* enumerate all divisors of a number from its prime factorisation;
   accumulate into *sum all (d+1) where d ≡ 3 (mod 4). */
static void enumerate_divisors(size_t idx, idx_t current,
                               const idx_t *primes, const int *exp,
                               size_t cnt, seq_t *sum) {
    if (idx == cnt) {
        if (current % 4 == 3) {
            *sum += (seq_t)current + (seq_t)1;
        }
        return;
    }
    idx_t p = primes[idx];
    idx_t val = 1;
    for (int e = 0; e <= exp[idx]; ++e) {
        enumerate_divisors(idx + 1, current * val, primes, exp, cnt, sum);
        val *= p;
    }
}

seq_t a(idx_t n) {
    /* n ≥ 1 by problem statement */
    if (n == 0) return (seq_t)0;   /* safety for out‑of‑range calls */
    ensure_primes();

    idx_t m = 4 * n - 2;
    idx_t temp = m;

    /* factorise m; at most a handful of prime factors for n ≤ 10 000 */
    idx_t pf[20];
    int   ex[20];
    size_t pf_cnt = 0;
    for (size_t i = 0; i < g_prime_cnt && (idx_t)g_primes[i] * g_primes[i] <= temp; ++i) {
        idx_t p = g_primes[i];
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            pf[pf_cnt] = p;
            ex[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (temp > 1) {
        pf[pf_cnt] = temp;
        ex[pf_cnt] = 1;
        ++pf_cnt;
    }

    seq_t sum = (seq_t)0;
    enumerate_divisors(0, (idx_t)1, pf, ex, pf_cnt, &sum);
    return sum / (seq_t)4;
}
