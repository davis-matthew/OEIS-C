/*
 * A295801  Numbers that have exactly four representations as a sum of seven
 *          positive squares.
 * https://oeis.org/A295801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   compute_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates the OEIS sequence A295801:
   a(n) = the n-th integer that can be expressed in exactly four ways
   as a sum of seven positive squares (unordered representations).
   The values are computed by dynamic programming. */

/* The following types are provided by the build system:
   idx_t  – unsigned integer type for indices (e.g. uint64_t)
   seq_t  – unsigned integer type for sequence values (uint8_t…uint64_t,
            unsigned __int128, or an arbitrary-precision integer)   */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "../utils/compute_terms.h"

static seq_t *terms = NULL;          /* array of terms (1‑based) */
static size_t terms_len = 0;         /* number of stored terms   */
static idx_t  cur_limit = 0;         /* current maximum n tested for DP  */
static const idx_t MAX_LIMIT = 200000; /* safety upper bound for limits  */

/* ensure that at least 'needed' terms are pre‑computed */
static void ensure_terms(size_t needed)
{
    if (terms_len >= needed)
        return;                               /* already enough */

    /* progressively enlarge the limit until enough terms are found
       or LIMIT is exceeded. */
    while (terms_len < needed && cur_limit < MAX_LIMIT) {
        /* next limit: double the current one, start at 1k if none yet */
        idx_t new_limit = (cur_limit == 0) ? 1000ULL : cur_limit * 2ULL;
        if (new_limit > MAX_LIMIT)
            new_limit = MAX_LIMIT;

        seq_t *new_terms = NULL;
        size_t new_len   = 0;
        compute_terms(new_limit, &new_terms, &new_len);

        /* dispose old terms and replace */
        free(terms);
        terms = new_terms;
        terms_len = new_len;
        cur_limit = new_limit;
    }
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n < 1 */

    /* on first call generate the initial table */
    if (terms == NULL) {
        const idx_t init_limit = 1000ULL;   /* small start */
        compute_terms(init_limit, &terms, &terms_len);
        cur_limit = init_limit;
    }

    /* enlarge table if necessary */
    if ((size_t)n > terms_len)
        ensure_terms(n);

    if ((size_t)n <= terms_len)
        return terms[(size_t)n - 1];
    else
        return 0;                /* sequence exhausted or beyond built range */
}
