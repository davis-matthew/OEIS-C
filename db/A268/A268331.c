/*
 * A268331  Squarefree numbers differing by more than 2 from any other
 *          squarefree number.
 * https://oeis.org/A268331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 423, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268331: Squarefree numbers differing by more than 2 from any other squarefree number.
   The implementation precomputes a sieve of squarefree numbers up to a fixed upper bound and scans
   for numbers whose nearest squarefree neighbour is at distance at least 3. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT 20000000U   /* Upper bound for precomputation. Adjust if more terms are needed. */

static uint8_t *squarefree = NULL;   /* 1 if index is squarefree, 0 otherwise. */
static uint8_t *prime_flag = NULL;   /* 1 if index is prime, 0 otherwise. */

/* Initialise the squarefree sieve once. */
static void init_squarefree(void)
{
    if (squarefree != NULL) return;   /* Already initialised. */

    squarefree = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
    prime_flag = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
    if (!squarefree || !prime_flag) abort();

    /* Start with all entries set to squarefree. */
    memset(squarefree, 1, (LIMIT + 1) * sizeof(uint8_t));
    /* Prime sieve to identify primes up to LIMIT. */
    memset(prime_flag, 1, (LIMIT + 1) * sizeof(uint8_t));
    prime_flag[0] = prime_flag[1] = 0;

    for (idx_t i = 2; (idx_t)i * i <= LIMIT; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = (idx_t)i * i; j <= LIMIT; j += i) {
                prime_flag[j] = 0;
            }
        }
    }

    /* Mark multiples of prime squares as non‑squarefree. */
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (prime_flag[p]) {
            idx_t sq = p * p;
            for (idx_t j = sq; j <= LIMIT; j += sq) {
                squarefree[j] = 0;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* Undefined for offset 0, return 0. */

    init_squarefree();

    idx_t count = 0;
    for (idx_t m = 1; m <= LIMIT; ++m) {
        if (!squarefree[m]) continue;

        /* Check the four neighbours for squarefree property. */
        int isolated = 1;
        if (m >= 3 && squarefree[m - 2]) isolated = 0;
        if (m >= 2 && squarefree[m - 1]) isolated = 0;
        if (m + 1 <= LIMIT && squarefree[m + 1]) isolated = 0;
        if (m + 2 <= LIMIT && squarefree[m + 2]) isolated = 0;

        if (isolated) {
            ++count;
            if (count == n) return (seq_t)m;
        }
    }

    /* If we reach here, the pre‑computed limit was insufficient. */
    /* Fallback: continue searching beyond the limit using trial factorisation
       (this part is rarely reached for the expected n values). */
    idx_t p = LIMIT + 1;
    while (1) {
        /* Test if p is squarefree */
        int sq_free = 1;
        for (idx_t d = 2; (idx_t)d * d <= p; ++d) {
            if (p % (d * d) == 0) {
                sq_free = 0;
                break;
            }
        }
        if (!sq_free) { ++p; continue; }

        /* Check neighbours within current bounds. */
        int isolated = 1;
        if (p >= 3 && (p - 2) <= LIMIT && squarefree[p - 2]) isolated = 0;
        if (p >= 2 && (p - 1) <= LIMIT && squarefree[p - 1]) isolated = 0;
        if (p + 1 <= LIMIT && squarefree[p + 1]) isolated = 0;
        if (p + 2 <= LIMIT && squarefree[p + 2]) isolated = 0;
        /* Neighbours beyond LIMIT are treated as non‑squarefree. */

        if (isolated) {
            ++count;
            if (count == n) return (seq_t)p;
        }
        ++p;
    }
}
