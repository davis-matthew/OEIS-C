/*
 * A247558  Smallest integer x > 0 such that the number of semiprimes in the
 *          interval (x/2, x] equals n.
 * https://oeis.org/A247558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 7549, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements A247558: 
   a(n) = smallest integer x > 0 such that the number of semiprimes in the
   interval (x/2, x] equals n. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Upper limit for precomputation – enough to cover n up to ~10000. */
#define LIMIT 1048576UL   /* 2^20 */

/* One‑time precomputation state. */
static bool initialized = false;
static uint32_t *spf      = NULL;          /* Smallest prime factors. */
static seq_t   *is_semiprime = NULL;       /* 1 if number is semiprime. */
static seq_t   *prefix     = NULL;          /* Prefix sum of semiprimes. */
static seq_t  *best       = NULL;          /* best[n] = minimal x. */
static idx_t    max_n     = 0;              /* Maximum n found. */

static void init(void) {
    if (initialized) return;
    initialized = true;

    /* Allocate sieve structures. */
    spf = (uint32_t*)malloc((LIMIT + 1UL) * sizeof(uint32_t));
    if (!spf) return;
    is_semiprime = (seq_t*)malloc((LIMIT + 1UL) * sizeof(seq_t));
    if (!is_semiprime) return;
    prefix = (seq_t*)malloc((LIMIT + 1UL) * sizeof(seq_t));
    if (!prefix) return;

    /* Smallest prime factor sieve. */
    for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    /* Detect semiprimes and build prefix sums. */
    is_semiprime[0] = 0; is_semiprime[1] = 0;
    prefix[0] = 0;
    for (idx_t n = 2; n <= LIMIT; ++n) {
        idx_t temp = n, cnt = 0;
        while (temp > 1) {
            ++cnt;
            temp /= spf[temp];
        }
        is_semiprime[n] = (cnt == 2) ? 1 : 0;
        prefix[n] = prefix[n - 1] + is_semiprime[n];
    }

    /* Scan once to find maximum semiprime count in any interval. */
    for (idx_t x = 1; x <= LIMIT; ++x) {
        seq_t diff = prefix[x] - prefix[x / 2];
        if ((idx_t)diff > max_n) max_n = (idx_t)diff;
    }

    /* Allocate best array and fill minimal x for each reachable n. */
    best = (seq_t*)calloc((size_t)(max_n + 1), sizeof(seq_t));
    if (!best) return;
    for (idx_t x = 1; x <= LIMIT; ++x) {
        seq_t diff = prefix[x] - prefix[x / 2];
        idx_t dn = (idx_t)diff;
        if (dn == 0 || best[dn] != 0) continue; /* already set or zero. */
        best[dn] = (seq_t)x;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;      /* Out of offset; undefined behaviour. */
    init();
    if (n > max_n) return 0;   /* No such number within precomputed range. */
    return best[n];
}
