/*
 * A287060  Primes a(n) such that a(1) = 2, a(2) = 3, and a(n) is the smallest
 *          prime greater than a(n-1) such that (a(n) - 1)/2 is not divisible
 *          by a(m) for all m < n.
 * https://oeis.org/A287060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A287060:
   a(1) = 2, a(2) = 3.
   For n > 2, a(n) is the smallest prime greater than a(n‑1)
   such that (a(n) – 1) / 2 is not divisible by any a(m) with 1 ≤ m < n. */

/* This implementation builds the sequence incrementally,
   reusing previously computed terms.  It works for any
   seq_t type supplied by the build system, and relies
   on the utility function is_prime_small() to test
   primality of 64‑bit candidates.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

static seq_t *seq = NULL;          /* array of sequence values */
static size_t seq_len = 0;         /* number of stored terms   */
static size_t seq_cap = 0;         /* capacity of seq array    */

/* Ensure seq has at least need elements (indexing from 1) */
static void ensure_seq(idx_t need)
{
    if (need == 0)
        return;
    if (seq_len >= (size_t)need)
        return;                      /* already have enough terms */

    /* Allocate or grow the sequence storage if needed */
    if (seq == NULL) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
    }
    if (seq_len == 0) {               /* initialize first two terms */
        seq[0] = (seq_t)2;
        seq[1] = (seq_t)3;
        seq_len = 2;
    }

    /* Current candidate starts just after the latest term */
    uint64_t cand = (uint64_t)seq[seq_len - 1] + 1;

    /* Generate further terms until we reach the requested index */
    while (seq_len < (size_t)need) {
        /* Find next prime greater than cand */
        while (true) {
            if (cand > 2 && (cand & 1) == 0) {   /* skip even numbers */
                cand++;
                continue;
            }
            if (is_prime_small(cand))
                break;
            cand++;
        }

        /* Check the divisibility condition */
        seq_t q = (seq_t)(cand - 1) / 2;
        bool ok = true;
        for (size_t i = 0; i < seq_len; ++i) {
            if (q % seq[i] == 0) { /* (a(n)–1)/2 divisible by earlier a(i) */
                ok = false;
                break;
            }
        }

        if (ok) {
            /* Grow storage if necessary */
            if (seq_len + 1 > seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 64;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)cand;   /* append new term */
        }

        cand++;   /* next candidate for the following prime search */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined for n < 1; follows OEIS offset */
    ensure_seq(n);
    return seq[(size_t)n - 1];
}
