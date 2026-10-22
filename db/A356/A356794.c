/*
 * A356794  Odd numbers that have at least one prime factor congruent to 1
 *          (mod 4) and at least one prime factor congruent to 3 (mod 4).
 * https://oeis.org/A356794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A356794 – odd composite numbers with at least one prime factor ≡1 (mod 4)
   and at least one prime factor ≡3 (mod 4).  The function searches for
   the required term by brute‑force scanning of odd integers.  It expands
   a smallest‑prime‑factor table as needed.  The algorithm is O(n) in
   the number of terms requested.

   No precomputed table of terms is used; the routine works for any
   n ≥ 1 and any order of calls. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_spf.h"

static idx_t *spf = NULL;       /* smallest‑prime‑factor table */
static idx_t spf_limit = 0;     /* largest index for which spf is valid */

/* Extend the SPF table to cover all numbers up to 'limit'.  The table
   is rebuilt from scratch for simplicity. */
static void extend_spf(idx_t limit)
{
    if (limit <= spf_limit) return;
    if (spf) free(spf);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* Return true if m has at least one prime factor ≡1 (mod 4) and at least
   one prime factor ≡3 (mod 4).  m is assumed to be odd and ≥ 3. */
static bool satisfies(uint64_t m)
{
    bool has1 = false, has3 = false;
    uint64_t rem = m;
    while (rem > 1) {
        uint64_t p = spf[rem];
        if (p == 0) p = rem;          /* safety for a prime > spf_limit */
        if ((p & 3) == 1) has1 = true;
        else if ((p & 3) == 3) has3 = true;
        while (rem % p == 0) rem /= p;
        if (has1 && has3) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined index – return 0 for safety */

    uint64_t candidate = 3;        /* first odd ≥3 */
    size_t found = 0;

    while (found < (size_t)n) {
        if (candidate > spf_limit) {
            /* Ensure the SPF table is large enough for 'candidate'. */
            extend_spf(candidate);
        }
        if (satisfies(candidate)) {
            ++found;
            if (found == (size_t)n) return (seq_t)candidate;
        }
        candidate += 2;            /* next odd integer */
    }

    /* The loop always returns; this line is only to satisfy the compiler. */
    return 0;
}
