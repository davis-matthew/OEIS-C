/*
 * A353081  Numbers whose squares have the first two digits the same as the
 *          next two digits.
 * https://oeis.org/A353081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth integer m ≥ 1 such that the first two decimal
 *            digits of m² equal the next two decimal digits of m²
 * Range:     u8 none, u16 n <= 1, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   append_candidate, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes required helpers */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

static seq_t *cache = NULL;      /* cached terms of the sequence   */
static size_t cache_len = 0;     /* number of cached terms         */
static size_t cache_cap = 0;     /* current capacity of cache      */
static idx_t next_candidate = 1; /* next integer m to test         */

/* Return the nth term of A353081 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)                /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    /* generate terms until we have at least n cached */
    while (cache_len < (size_t)n) {
        idx_t m = next_candidate++;            /* candidate integer          */
        seq_t m_big   = (seq_t)m;              /* promote to seq_t           */
        seq_t square  = m_big * m_big;         /* m²                          */

        /* number of decimal digits of m² */
        int dlen = digit_len(square);          /* returns int                 */
        if (dlen >= 4) {                       /* need at least 4 digits      */
            idx_t exp = (idx_t)(dlen - 4);      /* exponent for 10^(dlen-4)   */
            seq_t divisor = pow10(exp);        /* 10^(dlen-4) in seq_t form   */
            seq_t leading4 = square / divisor; /* most significant 4 digits  */

            /* compare the first two and the next two digits */
            if ((leading4 / 100) == (leading4 % 100)) {
                append_candidate(&cache, &cache_len, &cache_cap, m_big);
            }
        }
    }

    return cache[n - 1]; /* zero‑based array indexing */
}
