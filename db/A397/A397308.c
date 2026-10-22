/*
 * A397308  Positive numbers m such that m - 2^k is a prime for exactly two
 *          values of k > 0.
 * https://oeis.org/A397308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth odd integer m > 0 such that m‑2^k is a prime for
 *            exactly two values of k > 0. In other words, m has exactly two
 *            representations of the form m = p + 2^k with p prime
 * Range:     u8 n <= 37, u16 n <= 7708, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/pow2.h"

static seq_t *terms   = NULL;  /* Cached sequence values                */
static size_t terms_len = 0;   /* Number of terms currently cached      */
static size_t terms_cap = 0;   /* Capacity of the cached array          */
static seq_t next_candidate = 5; /* Last odd number checked (start before 7) */

seq_t a(idx_t n)
{
    /* Compute terms up to index n, memoizing results */
    while (terms_len < (size_t)n) {
        /* Move to the next odd candidate */
        next_candidate += 2;          /* odd numbers only */
        seq_t m = next_candidate;

        /* Count representations m = p + 2^k, k > 0, p prime */
        size_t count = 0;
        seq_t max_pow = m - 2; /* largest 2^k that can be subtracted from m */
        seq_t pow2k   = 2;     /* 2^1 */

        while (pow2k <= max_pow) {
            if (isPrimeGeneric(m - pow2k)) {
                if (++count > 2) break; /* early exit */
            }
            /* Safeguard against overflow of pow2k << 1 */
            if (pow2k > max_pow / 2) break;
            pow2k <<= 1;
        }

        if (count == 2) {
            /* Grow storage if necessary */
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = m;
        }
    }

    return terms[(size_t)(n - 1)];
}
