/*
 * A365830  Heinz numbers of incomplete integer partitions, meaning not every
 *          number from 0 to A056239(n) is the sum of some submultiset.
 * https://oeis.org/A365830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_incomplete
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates Heinz numbers of incomplete integer partitions
   by scanning the integers in increasing order, testing each for
   incompleteness, and returning the n-th found value (1‑based). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_incomplete.h"

/* ------------------------------------------------------------------ */
/*  Prime infrastructure                                              */
/* ------------------------------------------------------------------ */

static uint32_t *primes      = NULL;
static size_t   primes_cnt   = 0;
static unsigned *prime_index = NULL;  /* prime -> 1‑based index */
static size_t   prime_index_size = 0;
static bool     primes_initialized = false;

/* Simple sieve up to a fixed limit.  The limit is comfortably
   larger than any value we expect to factor in this problem. */
static void init_primes(void)
{
    if (primes_initialized) return;
    const size_t limit = 300000;              /* primes up to ~300k */
    bool *composite = (bool *)calloc(limit + 1, sizeof(bool));

    primes = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    primes_cnt = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (!composite[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i) {
                    composite[j] = true;
                }
            }
        }
    }

    prime_index_size = limit + 1;
    prime_index = (unsigned *)calloc(prime_index_size, sizeof(unsigned));
    for (size_t i = 0; i < primes_cnt; ++i) {
        prime_index[primes[i]] = (unsigned)(i + 1);   /* 1‑based index */
    }

    free(composite);
    primes_initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Helpers for factorisation and incompleteness test                 */
/* ------------------------------------------------------------------ */

static void factor_uint64(uint64_t n, unsigned *indices, unsigned *exps,
                          size_t *cnt)
{
    size_t loc_cnt = 0;
    uint64_t m = n;

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        if (m % p == 0) {
            size_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            indices[loc_cnt]   = prime_index[p];
            exps[loc_cnt]      = (unsigned)e;
            ++loc_cnt;
        }
    }
    if (m > 1) {                     /* m is prime */
        indices[loc_cnt]   = prime_index[(unsigned)m];
        exps[loc_cnt]      = 1;
        ++loc_cnt;
    }
    *cnt = loc_cnt;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; behaviour below is irrelevant */

    init_primes();

    idx_t found = 0;
    /* Start from 3; the first Heinz number that is *not* complete is 3 */
    for (uint64_t x = 3; ; ++x) {
        unsigned idxs[16], exps[16];
        size_t cnt;
        factor_uint64(x, idxs, exps, &cnt);
        if (is_incomplete(idxs, exps, cnt)) {
            ++found;
            if (found == n) return (seq_t)x;
        }
    }
}
