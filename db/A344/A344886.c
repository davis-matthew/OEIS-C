/*
 * A344886  a(n) is the smallest triangular number that is a multiple of the
 *          product of the members of the n-th pair of twin primes.
 * https://oeis.org/A344886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest triangular number that is a multiple of the
 *            * product of the members of the n‑th pair of twin primes
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes all primes up to a fixed limit big enough
 * for n <= 10000 (which is the bound promised by the specification).  The
 * twin prime pairs are stored in a static array.  For a given n the pair
 * (p, p+2) is retrieved, the parameter
 *
 *      k = (p-1)*(p+2)/2
 *
 * is computed and the triangular number k*(k+1)/2 is returned.  The code
 * uses only integer arithmetic and works for any integer type supplied by
 * the build for seq_t.  The preprocessing is done only once, so fluctuations
 * in call order do not affect correctness. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Size of the sieve used to generate primes.  2,500,000 is enough for the
 * first 10,000 twin prime pairs (there are > 15,000 of them). */
#define SIEVE_LIMIT 2500000

/* Static storage for the sieve results.  The variables are lazy–initialized. */
static bool sieve_ready = false;
static uint32_t *primes = NULL;      /* list of all primes up to SIEVE_LIMIT */
static size_t primes_cnt = 0;
static uint32_t *twin_first = NULL;  /* list of the first prime in each twin pair */
static size_t twin_cnt = 0;

/* Helper: perform the sieve once and fill the primes and twin_first arrays. */
static void init_sieve(void)
{
    if (sieve_ready) return;
    sieve_ready = true;

    /* ---------- Sieve of Eratosthenes ---------- */
    unsigned char *is_comp = (unsigned char *)calloc(SIEVE_LIMIT + 1, 1);
    if (!is_comp) exit(EXIT_FAILURE);

    size_t est_primes = SIEVE_LIMIT / 10;          /* rough estimate */
    primes = (uint32_t *)malloc(est_primes * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);

    size_t pc = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (pc >= est_primes) {
                est_primes <<= 1;
                primes = (uint32_t *)realloc(primes, est_primes * sizeof(uint32_t));
                if (!primes) exit(EXIT_FAILURE);
            }
            primes[pc++] = (uint32_t)i;
            if (i <= SIEVE_LIMIT / i) {
                for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    primes_cnt = pc;

    /* ---------- Extract twin primes ---------- */
    twin_cnt = 0;
    size_t est_twin = primes_cnt / 10;           /* rough estimate */
    twin_first = (uint32_t *)malloc(est_twin * sizeof(uint32_t));
    if (!twin_first) exit(EXIT_FAILURE);

    for (size_t i = 0; i + 1 < primes_cnt; ++i) {
        if (primes[i + 1] == primes[i] + 2) {
            if (twin_cnt >= est_twin) {
                est_twin <<= 1;
                twin_first = (uint32_t *)realloc(twin_first, est_twin * sizeof(uint32_t));
                if (!twin_first) exit(EXIT_FAILURE);
            }
            twin_first[twin_cnt++] = primes[i];
        }
    }

    free(is_comp);
}

/* Public entry point required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of range, behaviour unimportant */

    init_sieve();                /* ensure primes/twins are available */

    if ((size_t)n > twin_cnt)    /* safety – should not happen for n <= 10000 */
        return 0;

    uint32_t p = twin_first[(size_t)n - 1];
    uint32_t q = p + 2;          /* the twin prime partner */

    /* k = (p-1)*q / 2 */
    seq_t k = ((seq_t)(p - 1) * (seq_t)q) / 2;

    /* Triangular number: T = k*(k+1)/2  computed without overflow. */
    seq_t T;
    if ((k & 1) == 0)             /* k even -> (k/2)*(k+1) */
        T = (k / 2) * (k + 1);
    else                          /* k odd  -> k*((k+1)/2) */
        T = k * ((k + 1) / 2);

    return T;
}
