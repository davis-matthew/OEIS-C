/*
 * A257762  Numbers n with property that A062234(n) = A062234(n+1).
 * https://oeis.org/A257762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 4044, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257762
 * a(n) = the n-th index i (1‑based) such that
 * 2*prime(i) + prime(i+2) == 3*prime(i+1).
 * Here prime(k) is the k-th prime.
 * The function memoises primes and found indices.
 * It works for all n ≥ 1; if the pre‑computed prime limit is
 * exceeded the function returns 0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Large enough to contain the first 10 000 indices */
static const idx_t PRIME_LIMIT = 10000000ULL;

static idx_t *primes = NULL;          /* array of primes */
static size_t primes_cnt = 0;         /* number of primes stored */

/* Dynamic array of found indices (stored as uint64_t for speed) */
static idx_t *found_idx = NULL;       /* indices that satisfy the condition */
static size_t found_cnt = 0;          /* number of stored indices */
static size_t found_cap = 0;          /* current capacity of found_idx */

/* Static position from where we continue scanning primes */
static size_t scan_pos = 0;

/* Initialise prime list if not yet done */
static void init_primes(void)
{
    if (primes != NULL)
        return;  /* already initialised */

    idx_t limit = PRIME_LIMIT;
    size_t size = (size_t)(limit + 1);

    /* Sieve of Eratosthenes */
    bool *isComposite = (bool *)malloc(size);
    if (!isComposite)
        exit(EXIT_FAILURE);
    memset(isComposite, 0, size);

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!isComposite[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                isComposite[j] = true;
        }
    }

    /* Count primes */
    for (idx_t i = 2; i <= limit; ++i)
        if (!isComposite[i])
            primes_cnt++;

    /* Allocate array for primes */
    primes = (idx_t *)malloc(primes_cnt * sizeof(idx_t));
    if (!primes) {
        free(isComposite);
        exit(EXIT_FAILURE);
    }

    /* Fill array */
    size_t i = 0;
    for (idx_t n = 2; n <= limit; ++n)
        if (!isComposite[n])
            primes[i++] = n;

    free(isComposite);
}

/* Core function computing a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* undefined for n < 1 */

    init_primes();   /* ensure primes are available */

    /* Generate indices until we have at least n of them */
    while (found_cnt < n) {
        /* Scan primes from the last position */
        for (; scan_pos + 2 < primes_cnt; ++scan_pos) {
            /* Compute 2*prime(i)+prime(i+2) and 3*prime(i+1) using 64‑bit arithmetic
             * to avoid overflow even when seq_t is small (e.g. uint8_t). */
            unsigned long long left  = 2ULL * primes[scan_pos] + primes[scan_pos + 2];
            unsigned long long right = 3ULL * primes[scan_pos + 1];

            if (left == right) {
                /* Store the 1‑based index i+1 */
                if (found_cnt >= found_cap) {
                    size_t newcap = found_cap ? found_cap * 2 : 16;
                    idx_t *tmp = (idx_t *)realloc(found_idx, newcap * sizeof(idx_t));
                    if (!tmp) {
                        free(primes);
                        exit(EXIT_FAILURE);
                    }
                    found_idx = tmp;
                    found_cap = newcap;
                }
                found_idx[found_cnt++] = scan_pos + 1;
                /* Next scan position moves to the element after the current i */
                if (++scan_pos + 2 >= primes_cnt)   /* ensure we don't overflow next iteration */
                    scan_pos = primes_cnt;          /* will exit outer loop */
                break;  /* a new index found, stop inner loop to re‑check found_cnt */
            }
        }

        /* If we reached end of primes and still lack enough indices,
         * the preset limit is insufficient. */
        if (scan_pos + 2 >= primes_cnt)
            return 0;
    }

    return (seq_t)found_idx[n - 1];
}
