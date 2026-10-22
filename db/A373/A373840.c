/*
 * A373840  a(n) = 1 if A276150(n) is a multiple of 3, otherwise 0, where
 *          A276150 is the digit sum in the primorial base.
 * https://oeis.org/A373840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if the primorial‑base digit sum of n is a multiple of
 *            3, 0 otherwise
 * Range:     u8 n <= 9366, u16 n <= 9366, u32 n <= 9366, u64 n <= 9366,
 *            u128 n <= 9366, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


/* Cached primorials */
static uint64_t *primorials = NULL;   /* primorials[i] = i#   (0# = 1) */
static size_t  prim_count   = 0;      /* number of stored primorials (>=1) */
static idx_t   max_n_cached = 0;      /* largest n for which primorials were built */

/* Build primes up to limit and then construct the primorial chain up to n.
   The routine is called only when a larger n is requested. */
static void build_primorials(idx_t n)
{
    /* free previous chain */
    free(primorials);
    primorials   = NULL;
    prim_count   = 0;
    max_n_cached = 0;

    /* Sieve of Eratosthenes for primes up to n. */
    size_t limit = (size_t)n;
    char *mark = (char *)calloc(limit + 1, sizeof(char));
    if (!mark) return;          /* out of memory – fall back to empty chain */

    uint64_t *primes = NULL;
    size_t    prime_cnt = 0;

    for (size_t p = 2; p <= limit; ++p) {
        if (!mark[p]) {
            /* p is prime */
            ++prime_cnt;
            primes = (uint64_t *)realloc(primes, prime_cnt * sizeof(uint64_t));
            primes[prime_cnt - 1] = p;

            if (p > limit / p) continue;
            for (size_t k = p * p; k <= limit; k += p) mark[k] = 1;
        }
    }
    free(mark);

    /* Build primorials until the next one would exceed n. */
    primorials = (uint64_t *)malloc((prime_cnt + 1) * sizeof(uint64_t));
    if (!primorials) { free(primes); return; }

    primorials[0] = 1;                /* 0# = 1 */
    size_t i = 0;
    uint64_t prod = 1;
    while (i < prime_cnt) {
        uint64_t next = prod * primes[i];
        if (next > (uint64_t)n) break;
        ++i;
        primorials[i] = next;
        prod = next;
    }
    prim_count = i + 1;                /* i is last used index */

    free(primes);
    max_n_cached = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (primorials == NULL || n > max_n_cached) build_primorials(n);

    uint64_t remaining = (uint64_t)n;
    uint64_t digit_sum = 0;

    /* Traverse primorial digits from largest to smallest. */
    for (size_t idx = prim_count; idx > 0; --idx) {
        uint64_t weight = primorials[idx - 1];
        uint64_t digit  = remaining / weight;
        remaining      -= digit * weight;
        digit_sum      += digit;
    }

    return (seq_t)((digit_sum % 3 == 0) ? 1 : 0);
}
