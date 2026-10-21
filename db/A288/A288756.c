/*
 * A288756  Numerator of the volume of the "monic slice" of the d-th
 *          Chern-Vaaler star body.
 * https://oeis.org/A288756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Numerator of 2^n * Π_{j=1..⌊(n−1)/2⌋}
 *            (2*j/(2*j+1))^(n−2*j)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include necessary utilities */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Auxiliary data for prime factorisation */
#define LIM 20000            /* Enough for all j and 2*j+1 with n≤10000 */
#define MAXP 2500           /* Number of primes ≤ LIM (≈2262) */

static bool initialized = false;
static int spf[LIM + 1];          /* smallest prime factor */
static int prime_to_index[LIM + 1];
static int primes[MAXP];
static size_t prime_count = 0;
static int idx_two = -1;

/* Initialise primes and spf tables */
static void init_primes(void)
{
    for (int i = 0; i <= LIM; ++i) spf[i] = i;

    for (int i = 2; i * i <= LIM; ++i)
        if (spf[i] == i)
            for (int j = i * i; j <= LIM; j += i)
                if (spf[j] == j) spf[j] = i;

    for (int i = 2; i <= LIM; ++i)
        if (spf[i] == i) {
            prime_to_index[i] = prime_count;
            primes[prime_count++] = i;
        }

    idx_two = prime_to_index[2];
}

/* Helper to add factor exponents */
static void add_factor(int p_index, int k, int factor)
{
    if (factor > 0)
        ; /* nothing */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (!initialized) {
        init_primes();
        initialized = true;
    }

    idx_t d = n;                      /* d = n (sequence index) */
    idx_t e = (d - 1) / 2;            /* e = floor((d-1)/2) */

    /* exponents for primes (only non‑negative will contribute) */
    static int exp[MAXP];
    for (size_t i = 0; i < prime_count; ++i) exp[i] = 0;

    /* exponent of 2 from 2^d and from the product terms */
    idx_t sum_k = e * (d - e - 1);    /* Σ (d-2j) for j=1..e */
    uint64_t exp_two = (uint64_t)d + (uint64_t)sum_k;
    exp[idx_two] = (int)exp_two;

    /* Process numerator part j^k and denominator part (2j+1)^k */
    for (idx_t j = 1; j <= e; ++j) {
        idx_t k = d - 2 * j;         /* exponent in this term */

        /* Add factors of j (numerator) */
        int t = (int)j;
        while (t > 1) {
            int p = spf[t];
            int cnt = 0;
            while (t % p == 0) {
                t /= p;
                ++cnt;
            }
            exp[prime_to_index[p]] += cnt * (int)k;
        }

        /* Subtract factors of (2j+1) (denominator) */
        int t2 = (int)(2 * j + 1);
        while (t2 > 1) {
            int p = spf[t2];
            int cnt = 0;
            while (t2 % p == 0) {
                t2 /= p;
                ++cnt;
            }
            exp[prime_to_index[p]] -= cnt * (int)k;
        }
    }

    /* Build the result from positive exponents only */
    seq_t res = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        int e_exp = exp[i];
        if (e_exp > 0) {
            /* pow_seq expects (seq_t base, idx_t exp) */
            seq_t pw = seq_pow((seq_t)primes[i], (idx_t)e_exp);
            res = seq_mul(res, pw);
        }
    }
    return res;
}
