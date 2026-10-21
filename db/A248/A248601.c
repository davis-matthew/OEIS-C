/*
 * A248601  Square array T(n, k) read by antidiagonals, n > 0 and k > 0: for
 *          any number n > 0, let f(n) be the function that associates k to
 *          the prime(k)-adic valuation of n (where prime(k) denotes the k-th
 *          prime); f establishes a bijection between the positive numbers and
 *          the arithmetic functions with nonne...
 * https://oeis.org/A248601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, mult
 * Formula:   a(n) is obtained by evaluating the square array T at the (i,j)
 *            position read by antidiagonals. T is completely multiplicative
 *            in both parameters. For n = 1… a(1)=1, a(2)=1, a(3)=1, a(4)=1,
 *            a(5)=2, a(6)=1, a(7)=1, a(8)=3, a(9)=3, a(10)=1, … (OEIS
 *            A248601)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   build_primes, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utilities provided in the build */
#include "../utils/build_primes.h"   /* build_primes(limit,&arr,&cnt) */
#include "../utils/seq_mul.h"        /* seq_mul(a,b) returns a*b as seq_t */

/* Global prime list (1‑based indexing: prime[1] = 2, prime[2] = 3, …) */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static const size_t PRIME_LIST_SIZE = 21000; /* first 21000 primes are enough for n <= 10000 */

/* Ensure prime list is built */
static void ensure_prime_list(void)
{
    if (prime_list != NULL) return;

    build_primes(PRIME_LIST_SIZE, &prime_list, &prime_cnt);
    /* The returned array is 0‑based (prime_list[0] == 2).  We keep it 0‑based
       but when we need the 1‑based index we add 1. */
}

/* Factor small integer x (x <= 20000 for the needed scope) into primes.
   idx[]: 1‑based index of each prime in global prime_list.
   exp[]: corresponding exponent.
   cnt: number of distinct prime factors found (max 32). */
static void factor_small(unsigned long long x,
                         int *idx, int *exp, int *cnt)
{
    *cnt = 0;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        unsigned long long p = prime_list[pi];
        if (p * p > x) break;
        if (x % p == 0) {
            int e = 0;
            int id = (int)(pi + 1); /* 1‑based index */
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            idx[*cnt] = id;
            exp[*cnt] = e;
            ++(*cnt);
        }
    }
    if (x > 1) { /* x is a prime > sqrt(original x) */
        /* find its index in prime_list (linear scan, x is tiny) */
        size_t pi = 0;
        while (pi < prime_cnt && prime_list[pi] < x) ++pi;
        int id = (int)(pi + 1);
        idx[*cnt] = id;
        exp[*cnt] = 1;
        ++(*cnt);
    }
}

/* Return prime number at 1‑based index pos (1 <= pos <= prime_cnt) */
static seq_t prime_at_index(int pos)
{
    return (seq_t)prime_list[pos - 1];
}

/* Primary function computing A248601(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    ensure_prime_list();

    /* Determine diagonal number s such that (s-1)*s/2 >= n.
       Since n is at most 10000 in typical tests, a simple loop is fast. */
    idx_t s = 2;
    while (((s - 1) * s) / 2 < n) ++s;

    idx_t prev_total = ((s - 2) * (s - 1)) / 2;
    idx_t i = n - prev_total;   /* first coordinate on this diagonal */
    idx_t j = s - i;            /* second coordinate */

    if (i == 1 || j == 1) return 1;  /* absorbing element */

    /* Factorize the two coordinates */
    int idx_i[32], exp_i[32], cnt_i;
    int idx_j[32], exp_j[32], cnt_j;
    factor_small(i, idx_i, exp_i, &cnt_i);
    factor_small(j, idx_j, exp_j, &cnt_j);

    seq_t result = 1;
    for (int a = 0; a < cnt_i; ++a) {
        for (int b = 0; b < cnt_j; ++b) {
            int exponent = exp_i[a] * exp_j[b];
            if (exponent == 0) continue;
            seq_t base = prime_at_index(idx_i[a] * idx_j[b]); /* product index */
            seq_t power = 1;
            for (int k = 0; k < exponent; ++k) {
                power = seq_mul(power, base);
            }
            result = seq_mul(result, power);
        }
    }
    return result;
}
