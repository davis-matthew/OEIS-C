/*
 * A292570  Least k > 0 such that A171102(n) + A171102(k) is again a term of
 *          A171102, the pandigital numbers (having each digit from '0' to '9'
 *          at least once).
 * https://oeis.org/A292570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 108, u64 n <= 108, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula    : a(n) = min { k > 0 | A171102(k)+A171102(n) is pandigital } */
/*  Description: A171102 is the increasing sequence of all decimal numbers
 *               containing each digit 0‑9 at least once.
 *               This implementation lazily generates A171102 and then
 *               brute‑forces the smallest k for each queried n.  The generated
 *               pandigital values are stored in ascending order, so only the
 *               requested indices need to be generated.  No non‑standard
 *               libraries are used. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* global cache of generated pandigital values */
static seq_t *pand = NULL;       /* array of values of A171102 */
static size_t pand_count = 0;    /* number of values currently stored */
static seq_t next_candidate = (seq_t)1023456789ULL; /* first pandigital number */

/* ------------------------------------------------------------------ */
/*  Helper: check whether a number contains all digits 0‑9 at least once */
static inline bool is_pandigital(seq_t x)
{
    unsigned char present[10] = {0};
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        present[d] = 1;
        x /= 10;
    }
    for (int i = 0; i < 10; ++i)
        if (!present[i])
            return false;
    return true;
}

/* ------------------------------------------------------------------ */
/*  Ensure that at least `need` pandigital numbers are generated and
 *  stored.  Generates sequentially increasing integers beginning
 *  at 'next_candidate'.  Complexity is linear in the number of
 *  integers examined. */
static void ensure_pandigitals(size_t need)
{
    if (pand_count >= need)
        return;
    /* grow array incrementally; double capacity each time to amortize */
    size_t new_cap = (pand_count == 0) ? 128 : pand_count * 2;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(pand, new_cap * sizeof(seq_t));
    if (!tmp) abort();      /* out of memory – program termination */
    pand = tmp;

    while (pand_count < need) {
        if (is_pandigital(next_candidate))
            pand[pand_count++] = next_candidate;
        /* advance to next integer */
        next_candidate++;
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)    /* offset is 1; undefined for n==0, return 0 */
        return (seq_t)0;
    ensure_pandigitals((size_t)n);          /* obtain A171102(n) */
    seq_t target = pand[(size_t)n - 1];      /* A171102(n) */

    for (idx_t k = 1; ; ++k) {              /* search for minimal k */
        ensure_pandigitals((size_t)k);      /* obtain A171102(k) */
        seq_t cand = pand[(size_t)k - 1];   /* A171102(k) */
        seq_t sum = target + cand;
        if (is_pandigital(sum))
            return k;
    }
}
