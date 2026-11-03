/*
 * A326912  BII-numbers of pairwise intersecting set-systems with empty
 *          intersection.
 * https://oeis.org/A326912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A326912: nth integer whose BII set‑system is pairwise intersecting
   with an empty total intersection.  Brute‑force search. */

#include <stdint.h>
#include <stdbool.h>

/* Return true iff the BII set‑system of n is pairwise intersecting and has
   empty total intersection. */
static bool is_valid_BII(idx_t n)
{
    if (n == 0) return true;              /* Empty set‑system is accepted. */

    uint64_t masks[64];
    int mcnt = 0;

    idx_t tmp = n;
    while (tmp) {
        /* Position (1‑based) of the current set bit. */
        idx_t pos = (idx_t)__builtin_ctzll(tmp) + 1;
        tmp &= tmp - 1;                   /* clear lowest set bit. */

        /* Build mask of binary indices of 'pos'. */
        uint64_t mask = 0;
        idx_t t = pos;
        while (t) {
            /* shift by (position‑1) to encode the element. */
            mask |= 1ULL << ((__builtin_ctzll(t)));
            t &= t - 1;
        }
        masks[mcnt++] = mask;
    }

    /* Pairwise intersection test. */
    for (int i = 0; i < mcnt; ++i) {
        for (int j = i + 1; j < mcnt; ++j) {
            if ((masks[i] & masks[j]) == 0) return false; /* empty intersection */
        }
    }

    /* Total intersection test. */
    uint64_t total = masks[0];
    for (int i = 1; i < mcnt; ++i) total &= masks[i];
    if (total != 0) return false;        /* non‑empty total intersection */

    return true;
}

/* Sequence A326912: a(n) = n-th smallest integer whose BII set-system is
   pairwise intersecting with empty total intersection. n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;          /* First term is 0. */

    idx_t count     = 1;                  /* Already found 0. */
    idx_t candidate = 1;                  /* Next integer to test. */

    while (count < n) {
        if (is_valid_BII(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }
    return (seq_t)candidate;              /* Unreachable, keeps compiler satisfied. */
}
