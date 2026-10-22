/*
 * A366845  Number of integer partitions of n that contain at least one even
 *          part and whose halved even parts are relatively prime.
 * https://oeis.org/A366845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 44, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A366845
 *  Number of integer partitions of n that contain at least one even part
 *  and whose halved even parts are relatively prime.
 *
 *  The algorithm uses inclusion–exclusion on the gcd of the halved even
 *  parts.  For each divisor d of the gcd we compute partitions whose
 *  even parts are all multiples of 2d and subtract the partitions that
 *  use no even parts.  
 *
 *  C_d(n) = partitions of n where every even part is a multiple of 2d
 *          = sum_{m=0}^{⌊n/(2d)⌋} Odd[n-2dm] * P[m]
 *    (P = ordinary partition numbers, Odd = partition numbers into odd parts).
 *
 *  B_d(n) = partitions with at least one even part = C_d(n) – Odd[n].
 *
 *  a(n) = Σ_{d≥1} μ(d) · B_d(n)  ,  μ = Möbius function.
 *
 *  Only O(n log n) additions/multiplications are required per query.
 *  All helper tables are grown lazily as larger indices are requested.
 *
 *  The function can be called in any order and for any n ≥ 0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* types provided by the build system */
typedef uint64_t idx_t;   /* integer index type */

static seq_t *P = NULL;      /* ordinary partitions up to current N       */
static seq_t *Odd = NULL;    /* partitions into odd parts up to current N   */
static int   *mu = NULL;     /* Möbius function up to current N/2           */

static size_t P_size  = 0;  /* P[0 .. P_size-1]  */
static size_t Odd_size = 0; /* Odd[0 .. Odd_size-1] */
static size_t mu_size = 0;  /* mu[0 .. mu_size-1]   */
static idx_t  max_n_seen = 0; /* largest n for which tables are valid */

/* ------------------------------------------------------------------ */
/* Compute ordinary partitions up to limit: P[0..limit]              */
/* ------------------------------------------------------------------ */
static void compute_partitions(size_t limit)
{
    if (limit + 1 > P_size) {
        P = (seq_t*)realloc(P, (limit + 1) * sizeof(seq_t));
        if (!P) exit(EXIT_FAILURE);
        P_size = limit + 1;
    }
    if (limit + 1 > Odd_size) {
        Odd = (seq_t*)realloc(Odd, (limit + 1) * sizeof(seq_t));
        if (!Odd) exit(EXIT_FAILURE);
        Odd_size = limit + 1;
    }
    /* Fill new entries with zero */
    for (size_t i = 0; i <= limit; ++i) {
        P[i] = 0;
        Odd[i] = 0;
    }
    P[0] = 1;
    Odd[0] = 1;
    /* ordinary partitions */
    for (size_t part = 1; part <= limit; ++part) {
        for (size_t j = part; j <= limit; ++j) {
            P[j] += P[j - part];
        }
    }
    /* partitions into odd parts only */
    for (size_t part = 1; part <= limit; part += 2) {
        for (size_t j = part; j <= limit; ++j) {
            Odd[j] += Odd[j - part];
        }
    }
}

/* ------------------------------------------------------------------ */
/* Compute Möbius function up to limit (inclusive).  mu[0] unused.    */
/* ------------------------------------------------------------------ */
static void compute_mobius(size_t limit)
{
    if (limit + 1 > mu_size) {
        mu = (int*)realloc(mu, (limit + 1) * sizeof(int));
        if (!mu) exit(EXIT_FAILURE);
        mu_size = limit + 1;
    }
    mu[0] = 0;
    mu[1] = 1;
    for (size_t i = 2; i <= limit; ++i) mu[i] = 1;

    size_t *is_composite = (size_t*)calloc(limit + 1, sizeof(size_t));
    if (!is_composite) exit(EXIT_FAILURE);
    size_t *primes = (size_t*)malloc((limit + 1) * sizeof(size_t));
    if (!primes) exit(EXIT_FAILURE);
    size_t pcnt = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            primes[pcnt++] = i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            size_t p = primes[j];
            size_t ip = i * p;
            if (ip > limit) break;
            is_composite[ip] = 1;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }
    free(is_composite);
    free(primes);
}

/* ------------------------------------------------------------------ */
/* Ensure tables are computed up to n.  They are grown lazily.         */
/* ------------------------------------------------------------------ */
static void ensure_tables(idx_t n)
{
    if (n <= max_n_seen) return;

    size_t oldN = (size_t)max_n_seen;
    size_t newN = (size_t)n;
    compute_partitions(newN);
    compute_mobius(n / 2 + 1);  /* need μ up to floor(n/2) */

    max_n_seen = n;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    ensure_tables(n);

    seq_t result = 0;
    idx_t half_n = n / 2;

    for (idx_t d = 1; d <= half_n; ++d) {
        int mu_d = mu[d];
        if (mu_d == 0) continue;          /* no contribution */

        idx_t max_t = n / (2 * d);
        seq_t sum = 0;
        for (idx_t t = 1; t <= max_t; ++t) {
            idx_t rem = n - 2 * d * t;     /* remaining after even part contribution */
            seq_t prod = Odd[rem] * P[t];
            sum += prod;
        }
        if (mu_d > 0)
            result += sum;
        else
            result -= sum;                 /* mu_d == -1 */
    }
    return result;
}
