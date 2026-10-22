/*
 * A378618  Sum of nonsquarefree numbers between prime(n) and prime(n+1).
 * https://oeis.org/A378618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all nonsquarefree integers between prime(n) and
 *            prime(n+1) (exclusive)
 * Range:     u8 n <= 23, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

 /* Utility declarations */
static void init_primes(void);
static bool is_nonsquarefree(idx_t x);

/* --- static data --- */
static bool initialized = false;
static uint64_t *spf = NULL;            /* smallest prime factor table */
static idx_t *primes = NULL;            /* list of primes */
static size_t primes_cnt = 0;

/* --- constants --- */
#define MAX_BOUND 200000            /* enough for n up to about 10000 */

/* ------------------------------------------------------------------ */
/* Compute all primes up to MAX_BOUND and a small prime‑factor table. */
static void init_primes(void)
{
    if (initialized) return;
    initialized = true;

    /* Allocate and initialise smallest prime factor array. */
    spf = (uint64_t *)malloc((MAX_BOUND + 1) * sizeof(uint64_t));
    for (idx_t i = 0; i <= MAX_BOUND; ++i)
        spf[i] = i;

    for (idx_t i = 2; i * i <= MAX_BOUND; ++i)
        if (spf[i] == i)
            for (idx_t j = i * i; j <= MAX_BOUND; j += i)
                if (spf[j] == j) spf[j] = i;

    /* Allocate space for primes (reserve enough for all primes <= MAX_BOUND). */
    primes = (idx_t *)malloc((MAX_BOUND / 10 + 1) * sizeof(idx_t));
    primes_cnt = 0;

    for (idx_t i = 2; i <= MAX_BOUND; ++i)
        if (spf[i] == i) /* i is prime */
            primes[primes_cnt++] = i;
}

/* ------------------------------------------------------------------ */
/* Test whether x is not squarefree (has a squared prime factor).     */
static bool is_nonsquarefree(idx_t x)
{
    while (x > 1) {
        idx_t p = spf[x];
        idx_t cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
            if (cnt == 2) return true;   /* squared prime factor found */
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Main function implementing the sequence.                           */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* behaviour below offset is irrelevant */

    init_primes();

    /* Basic bounds check – assumes n is not larger than the list of primes. */
    if (n >= primes_cnt) return 0;

    seq_t sum = 0;
    idx_t p1 = primes[n - 1];    /* prime(n)   */
    idx_t p2 = primes[n];       /* prime(n+1) */

    for (idx_t x = p1 + 1; x < p2; ++x)
        if (is_nonsquarefree(x))
            sum += (seq_t)x;

    return sum;
}
