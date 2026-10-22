/*
 * A358136  Irregular triangle read by rows whose n-th row lists the partial
 *          sums of the prime indices of n (row n of A112798).
 * https://oeis.org/A358136
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A358136: the triangle read row‑wise,
   where each row consists of the partial sums of the
   prime indices (with multiplicity) of its row number. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

static idx_t *spf = NULL;        /* smallest prime factor array   */
static idx_t *prime_idx = NULL;  /* prime number -> its index     */
static const idx_t INIT_LIMIT = (idx_t)200000;
static bool initialized = false;

/* Initialize spf and prime index tables up to INIT_LIMIT */
static void init(void) {
    if (initialized) return;
    initialized = true;

    build_spf(INIT_LIMIT, &spf);

    /* Count primes to allocate prime_idx */
    idx_t count = 0;
    for (idx_t i = 2; i <= INIT_LIMIT; ++i) {
        if (spf[i] == i) count++;
    }
    prime_idx = (idx_t *)malloc((INIT_LIMIT + 1) * sizeof(idx_t));
    if (!prime_idx) exit(1);

    idx_t index = 1;
    for (idx_t i = 2; i <= INIT_LIMIT; ++i) {
        if (spf[i] == i) {
            prime_idx[i] = index;
            index++;
        } else {
            prime_idx[i] = 0;
        }
    }
}

/* Count total number of prime factors of x (with multiplicity) */
static inline size_t bigOmega(idx_t x) {
    size_t cnt = 0;
    while (x > 1) {
        idx_t p = spf[x];
        do {
            x /= p;
            cnt++;
        } while (x % p == 0);
    }
    return cnt;
}

/* Return the nth element of A358136 (n >= 2) */
seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;  /* undefined below offset */

    init();

    /* Find the row r that contains the index n */
    idx_t r = 2;
    size_t cum = 0;  /* total elements of rows < r */

    while (true) {
        size_t len = bigOmega(r);          /* elements in row r */
        if (cum + len + 1 >= n) break;    /* n falls in this row */
        cum += len;
        r++;
    }

    /* Position within the row (0‑based) */
    size_t pos = (size_t)(n - (cum + 2));

    /* Compute the partial sums for row r and return the pos‑th one */
    idx_t sum = 0;
    size_t cur = 0;
    idx_t temp = r;

    while (temp > 1) {
        idx_t p = spf[temp];
        size_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        idx_t idx = prime_idx[p];   /* index of prime p */

        for (size_t k = 0; k < e; ++k) {
            sum += idx;
            if (cur == pos) {
                return (seq_t)sum;
            }
            cur++;
        }
    }
    /* If pos was not found (shouldn't happen) */
    return (seq_t)0;
}
