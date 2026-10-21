/*
 * A286524  Denominator of the volume of the d-th Chern-Vaaler star body.
 * https://oeis.org/A286524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of 2^(n+1) * (n+1)^e * Π_{k=1..e}
 *            ((2k)^(n-2k)/(2k+1)^(n+1-2k)) where e = floor((n-1)/2)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
/* Utilities for arbitrary‑precision integer operations */
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* ------------------------------------------------------------------ */
/* Prime sieve up to a fixed limit (large enough for all needed primes) */
#define PRIME_LIMIT 20000
static int primes[PRIME_LIMIT];
static size_t prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited)
        return;
    static bool sieve[PRIME_LIMIT + 1];
    /* Mark all entries as prime first */
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;
        }
    }
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            primes[prime_cnt++] = i;
    primes_inited = true;
}

/* ------------------------------------------------------------------ */
/* Helper: add prime factors of num (with multiplicity mult) to the
   exponent table.  sign = +1 adds to denominator exponents,
   sign = -1 subtracts from them (numerator). */
static void add_factor(uint64_t num, long long mult, int sign,
                       int *prime_list, long long *exp_list, size_t *cnt)
{
    uint64_t n = num;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        int p = primes[i];
        int c = 0;
        while ((n % p) == 0) {
            ++c;
            n /= p;
        }
        if (c > 0) {
            size_t idx = 0;
            bool found = false;
            while (idx < *cnt) {
                if (prime_list[idx] == p) { found = true; break; }
                ++idx;
            }
            if (!found) {
                prime_list[*cnt] = p;
                exp_list[*cnt] = 0;
                ++(*cnt);
                idx = *cnt - 1;
            }
            exp_list[idx] += sign * (long long)c * mult;
        }
    }
    if (n > 1) { /* n is prime */
        int p = (int)n;
        size_t idx = 0;
        bool found = false;
        while (idx < *cnt) {
            if (prime_list[idx] == p) { found = true; break; }
            ++idx;
        }
        if (!found) {
            prime_list[*cnt] = p;
            exp_list[*cnt] = 0;
            ++(*cnt);
            idx = *cnt - 1;
        }
        exp_list[idx] += sign * mult;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t ONE = 1;
    if (n == 0UL)
        return ONE;                /* a(0) = 1 */

    init_primes();

    idx_t e = (n - 1) / 2;          /* floor((n-1)/2) */

    /* Prime → signed exponent table */
    int prime_list[4000];
    long long exp_list[4000];
    size_t cnt = 0;

    /* Denominator factors: (2k+1)^(n+1-2k)  */
    for (idx_t k = 1; k <= e; ++k) {
        idx_t exp_den = n + 1 - 2 * k;
        if (exp_den > 0)
            add_factor(2 * k + 1, (long long)exp_den, +1,
                       prime_list, exp_list, &cnt);
    }

    /* Numerator factors: (2k)^(n-2k)  */
    for (idx_t k = 1; k <= e; ++k) {
        idx_t exp_num = n - 2 * k;
        if (exp_num > 0)
            add_factor(2 * k, (long long)exp_num, -1,
                       prime_list, exp_list, &cnt);
    }

    /* Additional numerator terms: 2^(n+1)  and  (n+1)^e  */
    add_factor(2, (long long)(n + 1), -1,
               prime_list, exp_list, &cnt);
    add_factor((uint64_t)(n + 1), (long long)e, -1,
               prime_list, exp_list, &cnt);

    /* Build the denominator from positive exponents */
    seq_t den = ONE;
    for (size_t i = 0; i < cnt; ++i) {
        if (exp_list[i] > 0) {
            seq_t p = (seq_t)prime_list[i];
            seq_t p_pow = seq_pow(p, (idx_t)exp_list[i]);
            den = seq_mul(den, p_pow);
        }
    }
    return den;
}
