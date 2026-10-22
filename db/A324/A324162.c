/*
 * A324162  Number T(n,k) of set partitions of [n] where each subset is again
 *          partitioned into k nonempty subsets; triangle T(n,k), n>=0,
 *          0<=k<=n, read by rows.
 * https://oeis.org/A324162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the flattened triangle T(n,k) where
 *            T(n,k) is the number of set partitions of [n] such that each
 *            block is further partitioned into k non‑empty subsets. The
 *            triangle entry is defined by T(n,k) = Sum_{j=1..floor(n/k)}
 *            (k*j)! * S(n, k*j) / ((k!)^j * j!) with S(n,m) the St...
 * Range:     u8 n <= 28, u16 n <= 46, u32 n <= 121, u64 n <= 277,
 *            u128 n <= 742, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>     /* for bool */
#include <stddef.h>     /* for size_t */
#include <stdint.h>     /* for uint64_t */

#include "../utils/seq_pow.h"  /* seq_pow(seq_t base, idx_t exp) */

/* maximum n we will need (sufficient for index up to ~10^4) */
#define MAX_N 200
/* maximum number of primes <= MAX_N */
#define MAX_PRIMES 50

/* static buffers for primes and factorial exponent tables */
static int prime_count = 0;
static int primes[MAX_PRIMES];
static int fact_exp[MAX_N + 1][MAX_PRIMES];   /* fact_exp[n][pi] = exponent of primes[pi] in n! */

/* static buffer for Stirling numbers */
static seq_t stirling[MAX_N + 1][MAX_N + 1];

/* ensures that the prime list and factorial exponent tables are
   computed up to at least 'up_to' */
static void _ensure_primes_and_factorial_exp(size_t up_to)
{
    static size_t max_n = 0;
    if (up_to <= max_n)
        return;

    /* recompute primes up to 'up_to' */
    bool is_prime[MAX_N + 1];
    for (size_t i = 0; i <= up_to; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    prime_count = 0;
    for (size_t i = 2; i <= up_to; ++i) {
        if (!is_prime[i])
            continue;
        if (prime_count < MAX_PRIMES)
            primes[prime_count++] = (int)i;
        for (size_t j = i * i; j <= up_to; j += i)
            is_prime[j] = false;
    }

    /* initialise all exponent tables to zero */
    for (size_t i = 0; i <= up_to; ++i)
        for (int pi = 0; pi < prime_count; ++pi)
            fact_exp[i][pi] = 0;

    /* compute exponent tables incrementally */
    for (size_t n = 2; n <= up_to; ++n) {
        /* copy previous exponents */
        for (int pi = 0; pi < prime_count; ++pi)
            fact_exp[n][pi] = fact_exp[n - 1][pi];

        /* factor n and add exponents */
        size_t tmp = n;
        for (int pi = 0; pi < prime_count && (size_t)primes[pi] * (size_t)primes[pi] <= tmp; ++pi) {
            int p = primes[pi];
            while (tmp % p == 0) {
                fact_exp[n][pi]++;
                tmp /= p;
            }
        }
        if (tmp > 1) {               /* remaining prime factor */
            for (int pi = 0; pi < prime_count; ++pi) {
                if (primes[pi] == (int)tmp) {
                    fact_exp[n][pi]++;
                    break;
                }
            }
        }
    }

    max_n = up_to;
}

/* Computes T(row,k) for 0 <= k <= row (row > 0).  T(row,0) = 0 for
   row > 0; T(0,0) = 1 is handled outside this function. */
static seq_t _T_row_K(size_t row, size_t k)
{
    if (k == 0)
        return (seq_t)0;   /* T(n,0)=0 for n>0 */

    /* compute Stirling numbers of the second kind up to 'row' */
    for (size_t i = 0; i <= row; ++i)
        for (size_t j = 0; j <= row; ++j)
            stirling[i][j] = (seq_t)0;
    stirling[0][0] = (seq_t)1;
    for (size_t n = 1; n <= row; ++n) {
        for (size_t s = 1; s <= n; ++s) {
            stirling[n][s] = (seq_t)s * stirling[n - 1][s] + stirling[n - 1][s - 1];
        }
    }

    seq_t sum = (seq_t)0;
    size_t max_j = row / k;
    for (size_t j = 1; j <= max_j; ++j) {
        size_t m = k * j;               /* m = k*j */
        seq_t base = stirling[row][m];
        if (base == (seq_t)0)
            continue;

        /* compute combinatorial factor:
           comb = (k*j)! / ((k!)^j * j!) by prime exponents */
        int exps[MAX_PRIMES];
        for (int pi = 0; pi < prime_count; ++pi) {
            int e = fact_exp[m][pi] - (int)j * fact_exp[k][pi] - fact_exp[j][pi];
            exps[pi] = e;
        }
        seq_t comb = (seq_t)1;
        for (int pi = 0; pi < prime_count; ++pi) {
            int e = exps[pi];
            if (e > 0)
                comb *= seq_pow((seq_t)primes[pi], (idx_t)e);
        }

        sum += base * comb;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* map flat index n to (row, k) */
    size_t row = 0;
    while (((row + 1) * (row + 2)) / 2 <= (size_t)n)
        ++row;
    size_t rows_before = row * (row + 1) / 2;
    size_t k = (size_t)n - rows_before;

    if (row == 0 && k == 0)
        return (seq_t)1;   /* T(0,0) = 1 */

    _ensure_primes_and_factorial_exp(row);

    return _T_row_K(row, k);
}
