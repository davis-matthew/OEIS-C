/*
 * A342522  Heinz numbers of integer partitions with constant (equal) first
 *          quotients.
 * https://oeis.org/A342522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   constant_first_quotients
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/constant_first_quotients.h"

/* Algorithm:
   a(n) is the nth Heinz number of an integer partition whose prime indices
   (after sorting in decreasing order) form a geometric progression with a
   constant first quotient.  The function iterates through integers,
   factors each using a pre‑computed smallest‑prime‑factor array, builds the
   list of prime indices (with multiplicities), sorts it descending and
   checks whether all consecutive ratios are equal.  It memoises the
   sieve so that calls are independent of call order. */

#define INITIAL_SIEVE_LIMIT 200000ULL  /* sufficient for n ≤ 10000 */

static bool tables_inited = false;
static idx_t sieve_limit = 0;
static idx_t *spf = NULL;          /* smallest prime factor for each number   */
static idx_t *prime_idx = NULL;    /* 1‑based index of each prime, 0 otherwise */

static void init_tables(idx_t limit)
{
    if (tables_inited && limit <= sieve_limit)
        return;

    /* free old tables if re‑initialising */
    if (tables_inited) {
        free(spf);
        free(prime_idx);
        tables_inited = false;
    }

    sieve_limit = limit;
    spf = (idx_t *)calloc((size_t)(sieve_limit + 1), sizeof(idx_t));
    if (!spf) return;                            /* allocation failed */

    /* Sieve for smallest prime factor */
    for (idx_t i = 2; i <= sieve_limit; ++i) {
        if (spf[i] == 0) {                      /* i is prime */
            spf[i] = i;
            if (i * i <= sieve_limit) {
                for (idx_t j = i * i; j <= sieve_limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }

    /* Build array mapping prime → 1‑based index */
    prime_idx = (idx_t *)calloc((size_t)(sieve_limit + 1), sizeof(idx_t));
    if (!prime_idx) {
        free(spf); spf = NULL;
        return;
    }
    idx_t cnt = 0;
    for (idx_t i = 2; i <= sieve_limit; ++i)
        if (spf[i] == i) {                     /* prime */
            ++cnt;
            prime_idx[i] = cnt;
        }

    tables_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                      /* undefined, but safe */

    /* Initialise tables once.  Using a limit that comfortably exceeds
       expected maximum m for the required n.  The limit can be increased
       automatically if needed (here we keep it fixed). */
    init_tables(INITIAL_SIEVE_LIMIT);
    if (!tables_inited)
        return (seq_t)0;                      /* out of memory */

    /* Main search loop.  Start from 1 and count qualifying numbers. */
    idx_t count = 0;
    for (idx_t m = 1;; ++m) {
        /* Factor m into prime indices with multiplicity. */
        idx_t indices[64];
        size_t len = 0;
        idx_t temp = m;
        while (temp > 1 && len < sizeof(indices) / sizeof(indices[0])) {
            idx_t p = spf[temp];
            idx_t id = prime_idx[p];          /* 1‑based prime index */
            /* record multiplicity */
            while (temp % p == 0 && len < sizeof(indices) / sizeof(indices[0])) {
                indices[len++] = id;
                temp /= p;
            }
        }

        /* If the factorisation exceeded array size, skip this m.
           For the intended limits this does not happen. */
        if (len == 0) {                       /* m == 1 handles itself */
            if (++count == n)
                return (seq_t)m;
            continue;
        }

        /* Sort indices descending for the quotient test. */
        /* Simple bubble sort; size is tiny. */
        for (size_t i = 0; i + 1 < len; ++i)
            for (size_t j = i + 1; j < len; ++j)
                if (indices[j] > indices[i]) {
                    idx_t t = indices[i];
                    indices[i] = indices[j];
                    indices[j] = t;
                }

        if (constant_first_quotients(indices, len)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
