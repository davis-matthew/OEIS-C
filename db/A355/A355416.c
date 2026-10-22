/*
 * A355416  a(n) is the least k such that k divides Sum_{i=k..k+n-1}
 *          A001414(i).
 * https://oeis.org/A355416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1334, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A355416
 * a(n) = least k such that n divides Σ_{i=k}^{k+n−1} A001414(i)
 *   (A001414(i) = sum of the prime factors of i, counted with multiplicity).
 *
 * The algorithm precomputes all values of A001414 up to a fixed limit
 * using a linear sieve, then for each n scans k=1,2,… while keeping a
 * running window sum.  The window sum is updated in O(1) per step.
 *
 * The limit 5 000 000 is large enough for n≤10 000; it can be increased
 * if needed.  All calculations use the opaque type seq_t, which supports
 * ordinary arithmetic and modulo with the integer type idx_t.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--- Configuration -------------------------------------------------------*/

static const size_t SPF_LIMIT = 5000000;   /* sieve limit (>= expected max k+n) */

/*--- Global data --------------------------------------------------------*/

static uint32_t *spf      = NULL;  /* smallest prime factor of i (0/1 → 0) */
static uint32_t *pf_sum   = NULL;  /* A001414(i) for i <= SPF_LIMIT */
static size_t    primes_cap = 0;
static uint32_t *primes    = NULL;  /* list of primes produced by sieve */
static size_t    prime_cnt = 0;
static bool      sieve_done = false;

/*--- Helpers ------------------------------------------------------------*/

static void init_sieve(void)
{
    if (sieve_done) return;

    /* allocate memory for spf and pf_sum */
    spf    = (uint32_t *)calloc(SPF_LIMIT + 1, sizeof(uint32_t));
    pf_sum = (uint32_t *)calloc(SPF_LIMIT + 1, sizeof(uint32_t));
    if (!spf || !pf_sum) { perror("calloc"); exit(EXIT_FAILURE); }

    /* prime list for linear sieve */
    primes_cap = SPF_LIMIT / 10;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) { perror("malloc"); exit(EXIT_FAILURE); }

    pf_sum[0] = 0;
    pf_sum[1] = 0;

    for (uint32_t i = 2; i <= (uint32_t)SPF_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (prime_cnt == primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) { perror("realloc"); exit(EXIT_FAILURE); }
                primes = tmp;
            }
            primes[prime_cnt++] = i;
        }

        /* linear sieve */
        for (size_t j = 0; j < prime_cnt && primes[j] <= spf[i] && (uint64_t)i * primes[j] <= SPF_LIMIT; ++j) {
            uint32_t multi = i * primes[j];
            spf[multi] = primes[j];
        }

        /* compute A001414(i) using the recurrence */
        pf_sum[i] = pf_sum[i / spf[i]] + spf[i];
    }

    sieve_done = true;
}

/*--- Main sequence function ---------------------------------------------*/

seq_t a(idx_t n)
{
    /* ensure sieve is built once */
    init_sieve();

    /* n must be >=1 according to the offset */
    if (n == 0) return 0;  /* defensive, though not expected */

    uint64_t N = (uint64_t)n; /* avoid overflow when adding */
    seq_t sum = 0;

    /* initial window sum for k = 1 */
    for (uint64_t i = 1; i <= N; ++i) {
        sum += (seq_t)pf_sum[(size_t)i];
    }

    uint64_t k = 1;
    for (;;) {
        /* check divisibility by N */
        if (sum % (seq_t)N == 0) {
            return (seq_t)k;
        }

        /* slide window: remove f(k), add f(k+N) */
        uint32_t sub = pf_sum[(size_t)k];          /* k <= SPF_LIMIT */
        uint32_t add = pf_sum[(size_t)(k + N)];    /* k+N <= SPF_LIMIT because of chosen limit */

        sum -= (seq_t)sub;
        sum += (seq_t)add;

        ++k;
    }

    /* unreachable */
    return 0;
}
