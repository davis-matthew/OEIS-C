/*
 * A362933  a(n) = (A132188(n) - A362931(n))/2.
 * https://oeis.org/A362933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = (A132188(n) - A362931(n)) / 2
 * Range:     i8 n <= 48, i16 n <= 6458, i32 n <= 9988, i64 n <= 9988,
 *            i128 n <= 9988, big any n
 * Helpers:   coprime_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes the Möbius function and a list of primes up to n
 * using a linear sieve and then counts the required pairs in O(n log n) time. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/coprime_upto.h"

/* --- Static data for the Möbius function and primes --- */
static idx_t max_n = 0;          /* highest index processed so far   */
static int8_t *mu_arr = NULL;    /* Möbius values for 0..max_n       */
static int *primes   = NULL;     /* list of primes up to max_n       */
static size_t primes_cap = 0;    /* size of primes array             */
static size_t primes_cnt = 0;    /* number of primes stored          */

/* --- Integer square root by binary search --- */
static idx_t isqrt_idx(idx_t x)
{
    idx_t l = 0, r = x;
    while (l <= r) {
        idx_t m = (l + r) >> 1;
        if (m > 0 && m > x / m) {
            r = m - 1;
        } else {
            if ((m + 1) > 0 && (m + 1) > x / (m + 1))
                return m;
            l = m + 1;
        }
    }
    return l - 1;
}

/* --- Ensure μ array and prime list are built up to N --- */
static void ensure_sieve(idx_t N)
{
    if (N <= max_n)
        return;
    mu_arr  = (int8_t *)realloc(mu_arr, (size_t)(N + 1) * sizeof(int8_t));
    primes  = (int *)realloc(primes, (size_t)(N + 1) * sizeof(int));
    bool *is_comp = (bool *)calloc((size_t)(N + 1), sizeof(bool));

    mu_arr[0] = 0;               /* unused */
    mu_arr[1] = 1;

    size_t pc = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            primes[pc++] = (int)i;
            mu_arr[i] = -1;
        }
        for (size_t j = 0; j < pc && i * (idx_t)primes[j] <= N; ++j) {
            idx_t x = i * (idx_t)primes[j];
            is_comp[x] = true;
            if (i % primes[j] == 0) {
                mu_arr[x] = 0;
                break;
            } else {
                mu_arr[x] = -mu_arr[i];
            }
        }
    }
    free(is_comp);
    max_n = N;
    primes_cap = (size_t)(N + 1);
    primes_cnt = pc;
}

/* --- Factorize x into distinct prime factors (stored in pf) --- */
static void factor_unique(idx_t x, int *pf, size_t *pc)
{
    *pc = 0;
    for (size_t i = 0; i < primes_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= x; ++i) {
        int p = primes[i];
        if (x % p == 0) {
            pf[(*pc)++] = p;
            while (x % p == 0)
                x /= p;
        }
    }
    if (x > 1)
        pf[(*pc)++] = (int)x;
}

/* --- Count coprime ordered pairs (a,b) with a+b=s and a,b≤limit --- */
static seq_t count_coprime_interval(idx_t s, idx_t limit)
{
    if (s < 2)
        return 0;
    idx_t low  = (s > limit) ? s - limit : 1;
    idx_t high = (s - 1 < limit) ? (s - 1) : limit;
    if (low > high)
        return 0;

    int pf[16];
    size_t pc;
    factor_unique(s, pf, &pc);

    seq_t cnt_high = coprime_upto(high,   s, pf, pc);
    seq_t cnt_low  = coprime_upto(low-1, s, pf, pc);
    return cnt_high - cnt_low;
}

/* --- Main sequence function --- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Build sieve up to n */
    ensure_sieve(n);

    /* A132188(n): pairs with i*j a perfect square */
    seq_t geom_pairs = 0;
    for (idx_t r = 1; r <= n; ++r) {
        if (mu_arr[r] != 0) {
            idx_t q = isqrt_idx(n / r);
            seq_t qq = (seq_t)q;
            geom_pairs += qq * qq;
        }
    }

    /* A362931(n): pairs with integral harmonic mean */
    seq_t harm_pairs = 0;
    for (idx_t g = 1; g <= n; ++g) {
        idx_t limit = n / g;
        if (!limit)
            continue;
        idx_t two_g = 2 * g;
        for (idx_t d = 1; d * d <= two_g; ++d) {
            if (two_g % d == 0) {
                idx_t s1 = d;
                if (s1 >= 2 && s1 <= (idx_t)2 * limit)
                    harm_pairs += count_coprime_interval(s1, limit);
                idx_t s2 = two_g / d;
                if (s2 != s1 && s2 >= 2 && s2 <= (idx_t)2 * limit)
                    harm_pairs += count_coprime_interval(s2, limit);
            }
        }
    }

    return (geom_pairs - harm_pairs) / (seq_t)2;
}
