/*
 * A336099  Number of solutions of the equation k = n*sopf(k) in positive
 *          integers where sopf(k) is the sum of distinct prime factors of k.
 * https://oeis.org/A336099
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions of k = n * sopf(k) where sopf(k) is
 *            sum of distinct prime factors of k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Maximum s to scan; empirically covers all solutions for n <= 10000 */
#define MAX_S 200000

static bool initialized = false;
static uint32_t spf[MAX_S + 1];

/* Sieve to compute smallest prime factor for every integer up to MAX_S */
static void init_prime_sieve(void)
{
    if (initialized)
        return;
    for (int i = 0; i <= MAX_S; ++i)
        spf[i] = (uint32_t)i;
    for (int i = 2; i * i <= MAX_S; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (int j = i * i; j <= MAX_S; j += i) {
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
            }
        }
    }
    initialized = true;
}

/* Factor n into distinct primes, store in pr[10] and return count and sum of primes */
static void factor_n(uint64_t n, uint32_t *pr, int *cnt, uint64_t *sum)
{
    uint64_t tmp = n;
    int c = 0;
    uint64_t s = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            pr[c++] = (uint32_t)p;
            s += p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1) {
        pr[c++] = (uint32_t)tmp;
        s += tmp;
    }
    *cnt = c;
    *sum = s;
}

/* Return the number of solutions for given n */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    init_prime_sieve();

    /* Factor n */
    uint32_t primes_n[10];
    int cnt_n;
    uint64_t sum_n;
    factor_n(n, primes_n, &cnt_n, &sum_n);

    /* Count solutions */
    seq_t count = 0;
    for (int s = (int)sum_n; s <= MAX_S; ++s) {
        /* Compute sum of distinct primes of s that are NOT in primes_n */
        int tmp = s;
        int sum_q = 0;
        int prev_p = 0;
        while (tmp > 1) {
            int p = (int)spf[tmp];
            if (p == prev_p) {          /* skip duplicate exponent */
                while (tmp % p == 0)
                    tmp /= p;
                continue;
            }
            prev_p = p;
            bool common = false;
            for (int i = 0; i < cnt_n; ++i) {
                if (primes_n[i] == (uint32_t)p) {
                    common = true;
                    break;
                }
            }
            if (!common)
                sum_q += p;
            while (tmp % p == 0)
                tmp /= p;
        }

        if ((uint64_t)s == sum_n + sum_q)
            ++count;
    }

    return count;
}
