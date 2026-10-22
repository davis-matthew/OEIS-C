/*
 * A370363  Number A(n,k) of partitions of [k*n] into n sets of size k having
 *          at least one set of consecutive numbers whose maximum (if k>0) is
 *          a multiple of k; square array A(n,k), n>=0, k>=0, read by
 *          antidiagonals.
 * https://oeis.org/A370363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A370363(m,n) where m and n are obtained by reading the
 *            square array A(n,k) by antidiagonals. A370363(n,k) equals the
 *            number of partitions of [k*n] into n unlabeled sets of size k
 *            that contain at least one block of k consecutive numbers whose
 *            maximum is a multiple of k. For k>0 it is A37036...
 * Range:     u8 n <= 31, u16 n <= 40, u32 n <= 60, u64 n <= 96, u128 n <= 143,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

/* Prime sieve cache ------------------------------------------------------ */
static uint64_t *PRIMES = NULL;
static size_t PRIMES_CNT = 0;
static const idx_t PRIMES_MAX = 20000;      /* enough for (n,k)<=140*140 */

static void init_primes(void)
{
    if (PRIMES != NULL) return;
    build_primes_upto(PRIMES_MAX, &PRIMES, &PRIMES_CNT);
}

/* Exponent of prime p in m! ------------------------------------------------*/
static uint64_t exp_factorial(idx_t m, uint64_t p)
{
    uint64_t res = 0;
    uint64_t div = p;
    while (div <= (uint64_t)m) {
        res += m / div;
        div *= p;
    }
    return res;
}

/* A060540(n,k) : total number of partitions --------------------------------*/
static seq_t total_partitions(idx_t n, idx_t k)
{
    if (n == 0) return (seq_t)1;
    if (k == 0) return (seq_t)1;        /* by convention for n>0 */
    idx_t nk = n * k;
    seq_t r = (seq_t)1;

    for (size_t i = 0; i < PRIMES_CNT && PRIMES[i] <= nk; ++i) {
        uint64_t p = PRIMES[i];
        uint64_t e = exp_factorial(nk, p)
                   - (uint64_t)n * exp_factorial(k, p)
                   - exp_factorial(n, p);
        if (e == 0) continue;
        seq_t base = (seq_t)p;
        /* fast exponentiation */
        seq_t term = (seq_t)1;
        while (e > 0) {
            if (e & 1) term = term * base;
            base = base * base;
            e >>= 1;
        }
        r = r * term;
    }
    return r;
}

/* binomial coefficient C(n,t) using prime exponents -----------------------*/
static seq_t binomial_coeff(idx_t n, idx_t t)
{
    if (t > n) return (seq_t)0;
    seq_t r = (seq_t)1;
    idx_t k = t <= n - t ? t : n - t;   /* use symmetry */
    for (size_t i = 0; i < PRIMES_CNT && PRIMES[i] <= n; ++i) {
        uint64_t p = PRIMES[i];
        uint64_t e = exp_factorial(n, p) - exp_factorial(k, p) - exp_factorial(n - k, p);
        if (e == 0) continue;
        seq_t base = (seq_t)p;
        seq_t term = (seq_t)1;
        while (e > 0) {
            if (e & 1) term = term * base;
            base = base * base;
            e >>= 1;
        }
        r = r * term;
    }
    return r;
}

/* A370363(n,k) for k>0 ----------------------------------------------------*/
static seq_t A370363(idx_t n, idx_t k)
{
    /* special handling for k=0 */
    if (k == 0) return (n == 0) ? (seq_t)0 : (seq_t)1;

    seq_t sum = (seq_t)0;
    for (idx_t t = 1; t <= n; ++t) {
        seq_t binom = binomial_coeff(n, t);
        seq_t part = total_partitions(n - t, k);
        seq_t term = binom * part;
        if (t & 1)   /* odd t -> add */
            sum = sum + term;
        else         /* even t -> subtract */
            sum = sum - term;
    }
    return sum;
}

/* Antidiagonal coordinate extraction ---------------------------------------*/
static void antidiagonal_coords(idx_t flat, idx_t *pn, idx_t *pk)
{
    /* m is the diagonal number: largest m with m*(m+1)/2 <= flat */
    long double d = sqrt((long double)(8.0L * flat + 1.0L));
    idx_t m = (idx_t)((d - 1.0L) / 2.0L);
    while (m * (m + 1) / 2 > flat) --m;
    while ((m + 1) * (m + 2) / 2 <= flat) ++m;
    idx_t offset = flat - m * (m + 1) / 2;
    *pn = offset;
    *pk = m - offset;
}

/* Main sequence function ----------------------------------------------------*/
seq_t a(idx_t idx)
{
    init_primes();                     /* prime list ready */

    idx_t n, k;
    antidiagonal_coords(idx, &n, &k);
    if (n == 0 || k == 0) {
        /* special cases: a(0)=0; a(1)=0; a(3)=0 etc. */
        if (n == 0) return (seq_t)0;
        return A370363(n, k);          /* handles k=0 case */
    }
    return A370363(n, k);
}
