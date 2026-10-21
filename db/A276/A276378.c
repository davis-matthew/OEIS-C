/*
 * A276378  Numbers k such that 6*k is squarefree.
 * https://oeis.org/A276378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers k such that 6*k is squarefree
 * Range:     u8 n <= 19, u16 n <= 3788, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"

/* primes needed for square‑free tests */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* enlarge prime list to include all primes ≤ new_lim */
static void extend_primes(idx_t new_lim)
{
    if (new_lim <= prime_limit) return;
    uint64_t *new_pr;
    size_t new_cnt;
    build_primes_upto(new_lim, &new_pr, &new_cnt);
    if (primes) free(primes);
    primes   = new_pr;
    prime_cnt= new_cnt;
    prime_limit = new_lim;
}

/* test whether x is square‑free (uses the prime list) */
static bool is_squarefree(seq_t x)
{
    if (x <= 1) return true;
    seq_t v   = x;
    seq_t lim = floor_sqrt(v);
    if ((idx_t)lim > prime_limit) extend_primes((idx_t)lim);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > (uint64_t)lim) break;
        if (v % p == 0) {
            if ((v / p) % p == 0) return false;   /* p² | v */
            while (v % p == 0) v /= p;             /* remove all occurrences of p */
            lim = floor_sqrt(v);                   /* update limit after reduction */
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Sequence A276378: numbers k such that 6*k is squarefree,
       i.e. k is square‑free and coprime to 6. */
    if (n < 1) return 0;           /* undefined below offset 1 */

    idx_t count = 0;
    seq_t k = 1;
    while (count < n) {
        if ((k % 2) != 0 && (k % 3) != 0 && is_squarefree(k)) {
            ++count;
            if (count == n) return k;
        }
        ++k;
    }
    return 0;                       /* never reached */
}
