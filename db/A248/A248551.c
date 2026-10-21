/*
 * A248551  Numbers n such that the smallest prime divisor of n^2+1 is 89.
 * https://oeis.org/A248551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 250, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A248551: numbers n such that n^2 + 1 has smallest prime divisor 89.
   We iterate candidates n ≡ 34 or 144 (mod 178). For each candidate we test
   all primes p < 89; if any of them divides n^2 + 1, the candidate is discarded.
   Candidates for which the only prime divisor less than 89 is 89 are retained.
   The function caches already computed terms to support out‑of‑order calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_capacity.h"

static const uint64_t primes_lt_89[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23,
    29, 31, 37, 41, 43, 47, 53, 59,
    61, 67, 71, 73, 79, 83
};
static const size_t num_primes = sizeof(primes_lt_89) / sizeof(primes_lt_89[0]);

seq_t a(idx_t n)
{
    static seq_t *seq      = NULL;  /* cache of computed terms          */
    static size_t  size    = 0;     /* number of cached terms           */
    static size_t  capacity = 0;    /* allocated capacity for seq       */
    static seq_t  cur1 = (seq_t)34; /* next candidate in residue 34      */
    static seq_t  cur2 = (seq_t)144;/* next candidate in residue 144     */

    /* generate terms until we have at least n of them */
    while (size < n)
    {
        seq_t cand = (cur1 < cur2) ? cur1 : cur2;
        bool ok = true;

        /* test divisibility by all primes < 89 */
        for (size_t i = 0; i < num_primes && ok; ++i)
        {
            uint64_t p   = primes_lt_89[i];
            /* candidate may be bigger than 64 bits, but for n up to about 10⁴
               the values stay well inside 64‑bit, so we can safely cast. */
            uint64_t r   = (uint64_t)cand % p;
            uint64_t val = (r * r + 1) % p;
            if (val == 0)
                ok = false;
        }

        /* ensure that 89 indeed divides n²+1 */
        if (ok)
        {
            uint64_t r89 = (uint64_t)cand % 89ULL;
            if (((r89 * r89 + 1) % 89ULL) != 0)
                ok = false;
        }

        if (ok)
        {
            /* store the term, growing the cache if necessary */
            ensure_capacity(size + 1, &capacity, (void **)&seq, sizeof(seq_t));
            seq[size++] = cand;
        }

        /* advance the chosen sequence */
        if (cand == cur1)
            cur1 += (seq_t)178;
        else
            cur2 += (seq_t)178;
    }

    return seq[n - 1];
}
