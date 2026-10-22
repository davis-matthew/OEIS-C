/*
 * A374671  Positive numbers k such that k! and (k+1)! have an equal number of
 *          infinitary divisors.
 * https://oeis.org/A374671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) is the n-th positive integer k such that k! and (k+1)! have an equal
 * number of infinitary divisors.
 * The number of infinitary divisors of m equals
 *   2^(sum over primes p of popcount(v_p(m)))
 * where v_p(m) is the exponent of p in the prime factorization of m.
 * This implementation searches for such k incrementally, keeping a running
 * sum of the popcounts of the factorial exponents.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* maximum k that we will examine – enough for n up to about 10 000 */
#define MAX_K 2000000
/* maximum number of terms we will store – enough for n up to about 10 000 */
#define MAX_TERMS 50000

static uint32_t spf[MAX_K + 1];      /* smallest prime factor of each number */
static uint32_t exp_arr[MAX_K + 1];   /* current exponent of each prime in k! */
static seq_t   terms[MAX_TERMS];     /* stored a(n) values */
static size_t  terms_len   = 0;      /* number of terms stored so far */
static uint64_t current_sum = 0;     /* Σ popcount(v_p(k!)) for current k */
static idx_t   next_k      = 1;      /* next k to process (k!) */
static int     initialized = 0;      /* have we built the sieve etc. ? */

/* build spf array */
static void build_spf(void) {
    for (idx_t i = 0; i <= MAX_K; ++i) spf[i] = (uint32_t)i;
    for (idx_t i = 2; i * i <= MAX_K; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (idx_t j = i * i; j <= MAX_K; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }
    memset(exp_arr, 0, (MAX_K + 1) * sizeof(uint32_t));
}

/* compute the n-th term (n >= 1) */
seq_t a(idx_t n) {
    if (!initialized) {
        build_spf();
        initialized = 1;
    }

    while (terms_len < (size_t)n) {
        idx_t k = next_k;               /* current k (k! already represented) */
        uint64_t new_sum = current_sum;
        idx_t x = k + 1;                /* factorize (k+1) to update exponents */

        while (x > 1) {
            uint32_t p = spf[x];
            uint32_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            uint32_t old_exp = exp_arr[p];
            uint32_t new_exp = old_exp + e;
            new_sum += __builtin_popcount((unsigned int)new_exp) -
                       __builtin_popcount((unsigned int)old_exp);
            exp_arr[p] = new_exp;
        }

        if (new_sum == current_sum) {
            if (terms_len < (size_t)MAX_TERMS) {
                terms[terms_len++] = (seq_t)k;
            } else {
                /* unexpected: exceeding preallocated capacity */
                /* realloc not used to keep C compliance, silently ignore */
            }
        }

        current_sum = new_sum;
        ++next_k;
    }

    return terms[n - 1];
}
