/*
 * A353439  Integers m such that the decimal expansion of 1/m contains the
 *          digit 3.
 * https://oeis.org/A353439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   the nth term is the nth integer m such that the decimal
 *            expansion of 1/m contains digit 3. The function builds the
 *            sequence on demand: it keeps a cache of all terms found so far
 *            and extends it until the requested index is available
 * Range:     u8 n <= 14, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* test whether 1/m's decimal expansion contains digit 3 */
static int has_digit_3(seq_t m)
{
    if (m == 0)
        return 0;
    seq_t rem = 1 % m;
    size_t mm = (size_t)m;               /* assume m fits into size_t (true for the n ≤ 10000 range) */
    for (size_t i = 0; i < mm; ++i) {
        rem *= 10;
        seq_t d = rem / m;
        if (d == 3)
            return 1;
        rem %= m;
    }
    return 0;
}

static seq_t *terms = NULL;          /* cache of terms */
static size_t terms_sz = 0;          /* number of terms stored */
static seq_t next_candidate = 1;     /* next m to test */

seq_t a(idx_t n)
{
    /* make sure cache has at least n terms */
    while (terms_sz < (size_t)n) {
        /* find next m satisfying the condition */
        while (!has_digit_3(next_candidate))
            ++next_candidate;

        /* append to cache */
        seq_t *new_terms = mem_resize(seq_t, terms, (terms_sz + 1));
        if (!new_terms) {
            /* allocation failure – propagate 0 (caller must not expect success in this case) */
            return 0;
        }
        terms = new_terms;
        terms[terms_sz++] = next_candidate++;

        /* advance to next candidate for the next loop iteration */
    }
    return terms[n - 1];
}
