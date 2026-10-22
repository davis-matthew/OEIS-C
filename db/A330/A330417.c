/*
 * A330417  Coefficient of e(y) in Sum_{k > 0, i > 0} x_i^k = p(1) + p(2) +
 *          p(3) + ..., where e is the basis of elementary symmetric
 *          functions, p is the basis of power-sum symmetric functions, and y
 *          is the integer partition with Heinz number n.
 * https://oeis.org/A330417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(A056239(n)-Omega(n)) * A056239(n) * (Omega(n)-1)! /
 *            ∏_p (exp_p!) (p runs over prime factors of n)
 * Range:     i8 n <= 47, i16 n <= 9999, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   small_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A056239(n) = Σ exp_p * index(p)  and Omega(n)=Σ exp_p  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/small_factorial.h"

/* small prime sieve for factorisation (enough for n up to ~100000) */
#define PRIME_CAP 9600
static uint64_t prime_list[PRIME_CAP];
static size_t   prime_cnt   = 0;
static bool     primes_built = false;

static void build_primes(void)
{
    if (primes_built) return;
    primes_built = true;
    bool composite[100001] = {0};
    for (size_t i = 2; i * i <= 100000; ++i)
        if (!composite[i])
            for (size_t j = i * i; j <= 100000; j += i)
                composite[j] = true;
    prime_cnt = 0;
    for (size_t i = 2; i <= 100000; ++i)
        if (!composite[i])
            prime_list[prime_cnt++] = i;
}

/* return the 1‑based index of prime p in prime_list (assumes p is prime) */
static inline size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo + 1;   /* 1‑based index */
}

/* main function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1U) return 0;            /* a(1)=0 */

    build_primes();

    uint64_t temp = (uint64_t)n;
    idx_t    omega = 0;          /* total multiplicity of prime factors */
    seq_t   sumIdx = 0;          /* Σ index(p) * exp */
    seq_t   prodFac = 1;         /* product of exp! */

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            size_t idx = prime_index(p);
            omega += (idx_t)exp;
            sumIdx += (seq_t)idx * (seq_t)exp;
            prodFac *= small_factorial((unsigned)exp);
        }
    }

    if (temp > 1) {                       /* remaining prime factor */
        uint64_t p = temp;
        size_t idx = prime_index(p);
        omega += 1;
        sumIdx += (seq_t)idx;
        /* exp = 1 → 1! = 1; prodFac unchanged */
    }

    /* sign = (-1)^(sumIdx - omega) */
    seq_t  sumIdxMinusOmega = sumIdx - (seq_t)omega;
    bool   negative         = ((sumIdxMinusOmega % 2) != 0);

    /* (omega - 1)! */
    seq_t factOmegaMinusOne = 1;
    for (idx_t t = 2; t < omega; ++t)
        factOmegaMinusOne *= (seq_t)t;

    /* compute numerator * denominator ratio */
    seq_t result = sumIdx;
    result *= factOmegaMinusOne;
    result /= prodFac;        /* division is exact */

    if (negative) result = -result;

    return result;
}
