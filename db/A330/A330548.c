/*
 * A330548  Gasket based on primes: take array read by diagonals from
 *          Gilbreath conjecture (A036262) and replace all nonzero entries
 *          with 1's.
 * https://oeis.org/A330548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A330548 by constructing the Gilbreath array
   from primes, reading it by antidiagonals (bottom‑to‑top), and
   replacing non‑zero entries with 1. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* determine the diagonal index s such that we have more than n terms */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) {
        s++;
    }

    /* generate enough primes for this diagonal */
    size_t prime_cnt = 0;
    uint64_t *primes = NULL;
    idx_t limit = (idx_t)_prime_limit_for_idx(s);
    if (limit < (idx_t)s) limit = (idx_t)s * 20 + 10;   /* fallback */
    build_primes_upto(limit, &primes, &prime_cnt);
    while (prime_cnt <= (size_t)s) {
        /* not enough primes, increase limit and redo */
        free(primes);
        limit = limit * 2 + 10;
        build_primes_upto(limit, &primes, &prime_cnt);
    }

    /* build the Gilbreath table up to row s */
    size_t rows = (size_t)(s + 1);
    uint64_t **table = (uint64_t **)malloc(rows * sizeof(uint64_t *));
    if (!table) { free(primes); return 0; }

    table[0] = (uint64_t *)malloc((s + 1) * sizeof(uint64_t));
    for (size_t j = 0; j <= (size_t)s; j++) {
        table[0][j] = primes[j];
    }

    for (size_t i = 1; i <= (size_t)s; i++) {
        size_t len_prev = (s + 1) - (i - 1);
        size_t len = len_prev - 1;
        table[i] = (uint64_t *)malloc(len * sizeof(uint64_t));
        for (size_t j = 0; j < len; j++) {
            uint64_t a = table[i - 1][j];
            uint64_t b = table[i - 1][j + 1];
            table[i][j] = (b >= a) ? b - a : a - b;
        }
    }

    /* traverse the table by antidiagonals (bottom‑to‑top) */
    idx_t idx = 0;
    for (idx_t diag = 0; diag <= s; diag++) {
        for (idx_t i = diag; ; i--) {
            idx_t j = diag - i;
            uint64_t val = table[i][j];
            if (idx == n) {
                seq_t result = (val == 0) ? 0 : 1;
                /* free memory */
                for (idx_t k = 0; k <= (idx_t)s; k++) free(table[k]);
                free(table);
                free(primes);
                return result;
            }
            idx++;
            if (i == 0) break;
        }
    }

    /* should not reach here */
    for (idx_t k = 0; k <= (idx_t)s; k++) free(table[k]);
    free(table);
    free(primes);
    return 0;
}
