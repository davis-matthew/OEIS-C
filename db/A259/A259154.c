/*
 * A259154  Number of values v whose pi-based arithmetic derivative equals n.
 * https://oeis.org/A259154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ v ≥ 0 : A258851(v) = n }| (pi‑based arithmetic
 *            derivative)
 * Range:     u8 none, u16 none, u32 n <= 9951, u64 n <= 9951, u128 n <= 9951,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *counts = NULL;
static size_t max_D_global = 0;
static bool initialized = false;

static void init(void)
{
    const size_t LIMIT = 2000000;          /* sufficient for n up to ~1.5e5, well beyond 10000 */
    uint32_t *spf = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    uint32_t *pi   = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    seq_t    *D    = (seq_t*)malloc((LIMIT + 1) * sizeof(seq_t));

    if (!spf || !pi || !D) {
        /* allocation failure – abort to avoid undefined behaviour */
        abort();
    }

    /* linear sieve for spf and π(x) */
    size_t prime_capacity = 200000;
    uint32_t *primes = (uint32_t*)malloc(prime_capacity * sizeof(uint32_t));
    if (!primes) abort();
    size_t primes_cnt = 0;

    pi[0] = 0;
    pi[1] = 0;
    spf[0] = spf[1] = 0;

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (primes_cnt == prime_capacity) {
                prime_capacity *= 2;
                primes = (uint32_t*)realloc(primes, prime_capacity * sizeof(uint32_t));
                if (!primes) abort();
            }
            primes[primes_cnt++] = (uint32_t)i;
        }

        size_t j;
        for (j = 0; j < primes_cnt && primes[j] <= spf[i] && (uint64_t)i * primes[j] <= LIMIT; ++j) {
            spf[i * primes[j]] = primes[j];
        }

        pi[i] = pi[i - 1] + (spf[i] == (uint32_t)i ? 1 : 0);
    }

    /* compute the pi‑based arithmetic derivative */
    D[0] = 0;
    D[1] = 0;
    size_t max_D = 0;

    for (size_t n = 2; n <= LIMIT; ++n) {
        if (spf[n] == (uint32_t)n) {
            D[n] = (seq_t)pi[n];
        } else {
            uint32_t p = spf[n];
            size_t m = n / p;
            D[n] = (seq_t)pi[p] * (seq_t)m + (seq_t)p * D[m];
        }
        if ((size_t)D[n] > max_D) max_D = (size_t)D[n];
    }

    /* build frequency table */
    seq_t *freq = (seq_t*)calloc(max_D + 1, sizeof(seq_t));
    if (!freq) abort();

    for (size_t v = 0; v <= LIMIT; ++v) {
        size_t d = (size_t)D[v];
        freq[d] += (seq_t)1;
    }

    /* store results globally */
    counts = freq;
    max_D_global = max_D;

    /* free temporary arrays */
    free(spf);
    free(pi);
    free(D);
    free(primes);
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init();
        initialized = true;
    }
    if ((size_t)n <= max_D_global)
        return counts[(size_t)n];
    else
        return (seq_t)0;
}
