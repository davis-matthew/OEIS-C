/*
 * A375927  Numbers k such that A005117(k+1) - A005117(k) = 1. In other words,
 *          the k-th squarefree number is 1 less than the next.
 * https://oeis.org/A375927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A375927: Numbers k such that the k‑th square‑free number is
 *  one less than the next square‑free number.
 *
 *  This implementation uses a simple progressive sieve over the
 *  integers, storing the indices k in a dynamically resizable array.
 *  It uses the arbitrary‑precision helpers seq_alloc and seq_realloc
 *  for memory management so that it compiles both in C and C++.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_squarefree.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* static state for memoisation of the sequence */
static seq_t *kseq = NULL;   /* array holding the sequence values (k) */
static size_t klen = 0;      /* number of terms stored so far */
static size_t kcap = 0;      /* allocated capacity of kseq */

static idx_t pos_sqfree = 0;   /* position of current square‑free number (1‑based) */
static seq_t prev_sqfl = 0;    /* previous square‑free value encountered */
static idx_t test_val = 0;     /* integer which is being tested */

/* Grow the sequence array when needed using the provided utility */
static void grow_seq(void)
{
    size_t newcap = (kcap == 0) ? 16 : (kcap << 1);
    seq_t *tmp = seq_realloc(kseq, kcap, newcap);
    if (!tmp) abort();          /* out of memory */
    kseq = tmp;
    kcap = newcap;
}

/* Return the n‑th term (1‑based) of A375927.
 * The function is thread‑unsafe but memoises results in static
 * storage so subsequent calls are cheap even if unordered. */
seq_t a(idx_t n)
{
    /* Ensure the underlying array has been allocated once */
    if (!kseq)
        kseq = seq_alloc(kcap = 16);

    /* Generate terms until we have at least n of them. */
    while (klen < (size_t)n) {
        /* Test the next integer. */
        ++test_val;
        seq_t val = (seq_t)test_val;

        if (is_squarefree(val)) {
            /* New square‑free number found. */
            ++pos_sqfree;

            /* If it is exactly 1 larger than the previous
               square‑free number, record the index
               of the previous one (k = pos_sqfree - 1). */
            if (pos_sqfree > 1 && val == prev_sqfl + 1) {
                if (klen == kcap) grow_seq();
                kseq[klen++] = (seq_t)(pos_sqfree - 1);
            }

            /* Remember this value for the next comparison. */
            prev_sqfl = val;
        }
    }

    return kseq[n - 1];
}
