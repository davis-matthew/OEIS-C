/*
 * A377783  Least nonsquarefree number > prime(n).
 * https://oeis.org/A377783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least non‑squarefree number greater than prime(n)
 * Range:     u8 n <= 49, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define PRIME_LIMIT 200000  /* enough for n up to ~20000 (>10000) */

/* one‑time sieve of Eratosthenes */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_inited = false;

static void init_primes(void) {
    if (primes_inited) return;
    const size_t LIMIT = PRIME_LIMIT;
    uint8_t *mark = (uint8_t*)calloc(LIMIT + 1, 1);          /* 0 = prime, 1 = composite */
    if (!mark) exit(1); /* allocation failure */
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (!mark[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                mark[j] = 1;
        }
    }

    /* estimate number of primes up to LIMIT to allocate array */
    size_t approx = LIMIT / 10 + 10;  /* over‑estimate, safe */
    primes = (uint32_t*)malloc(approx * sizeof(uint32_t));
    if (!primes) { free(mark); exit(1); }

    primes_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!mark[i]) {
            if (primes_cnt >= approx) {   /* re‑allocate if needed */
                approx <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(primes, approx * sizeof(uint32_t));
                if (!tmp) { free(mark); free(primes); exit(1); }
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    }
    free(mark);
    primes_inited = true;
}

/* test if a number m (seq_t) is squarefree */
static bool is_squarefree(seq_t m) {
    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t p = primes[i];
        seq_t sq = p * p;
        if (sq > m) break;
        if (m % sq == 0) return false;
    }
    return true;
}

/* The required OEIS sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1, safety fallback */

    init_primes();

    if ((idx_t)primes_cnt <= n - 1)
        return 0;                  /* insufficient primes, safety fallback */

    seq_t prime_n = (seq_t)primes[n - 1];
    seq_t m = prime_n + 1;
    while (true) {
        if (!is_squarefree(m)) return m;
        ++m;
    }
}
