/*
 * A265709  a(n) = numerator of Sum_{d|n} 1/sigma(d).
 * https://oeis.org/A265709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{d|n} 1/sigma(d)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 8191,
 *            u128 n <= 8943, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Greatest common divisor for seq_t (works for all integer types) */
static seq_t my_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Factorise n into primes and exponents.
   primes[16] and exps[16] are filled; *cnt returns the number of distinct primes. */
static void factor_uint64(uint64_t n, uint64_t primes[16], int exps[16], size_t *cnt)
{
    size_t i = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[i] = p;
            exps[i] = e;
            ++i;
        }
    }
    if (tmp > 1) {            /* remaining prime factor */
        primes[i] = tmp;
        exps[i] = 1;
        ++i;
    }
    *cnt = i;
}

/* Recursive DFS over exponents of prime factors.
   cur_sigma_d is the sigma value of the divisor built so far. */
static void dfs(int idx, int num,
                const uint64_t primes[16], const int exps[16],
                const seq_t sigma_tbl[16][16],
                seq_t cur_sigma_d,
                seq_t *sum_num, seq_t *sum_den)
{
    if (idx == num) {
        /* Add the fraction 1 / cur_sigma_d to the running sum */
        seq_t new_num = (*sum_num) * cur_sigma_d + (*sum_den);
        seq_t new_den = (*sum_den) * cur_sigma_d;
        seq_t g = my_gcd(new_num, new_den);
        *sum_num = new_num / g;
        *sum_den = new_den / g;
        return;
    }
    int max_e = exps[idx];
    for (int e = 0; e <= max_e; ++e) {
        seq_t new_sigma = cur_sigma_d * sigma_tbl[idx][e];
        dfs(idx + 1, num, primes, exps, sigma_tbl,
            new_sigma, sum_num, sum_den);
    }
}

seq_t a(idx_t n)
{
    /* Special case n == 0 is outside the OEIS offset; just return 0. */
    if (n == 0) return 0;

    uint64_t primes[16];
    int exps[16];
    size_t num = 0;
    factor_uint64((uint64_t)n, primes, exps, &num);

    /* Pre‑compute sigma values for each prime power up to its exponent. */
    seq_t sigma_tbl[16][16];
    for (int i = 0; i < num; ++i) {
        seq_t p = (seq_t)primes[i];
        sigma_tbl[i][0] = 1;                 /* sigma(p^0) */
        for (int e = 1; e <= exps[i]; ++e) {
            sigma_tbl[i][e] = sigma_tbl[i][e - 1] * p + 1;
        }
    }

    seq_t sum_num = 0;
    seq_t sum_den = 1;

    dfs(0, (int)num, primes, exps, sigma_tbl, (seq_t)1, &sum_num, &sum_den);

    return sum_num;
}
