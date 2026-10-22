/*
 * A307854  Numbers k such that phi(r) + phi(s) = k has no solution, where phi
 *          is Euler totient function.
 * https://oeis.org/A307854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A307854: numbers k such that φ(r) + φ(s) = k has no solution.
   The algorithm precomputes Euler totients up to a fixed LIMIT,
   builds a set of all totient values, and then collects the first
   TARGET missing odd numbers (plus 1).  The results are cached for
   subsequent calls.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ----- static caching state ------------------------------------------ */
static seq_t   *seq_cache = NULL;   /* sequence values   */
static size_t   seq_len   = 0;      /* number of cached terms   */
static bool     init_done = false; /* initialization flag   */

/* ----- helper: bit test for the totient set -------------------------- */
static inline bool bit_is_set(const uint64_t *bits, uint32_t val)
{
    return (bits[val >> 6] >> (val & 63)) & 1ULL;
}

/* ----- initialise sequence: compute totients and missing numbers ---- */
static void init_sequence(void)
{
    /* parameters: enough to store >10 000 terms. */
    const idx_t LIMIT  = 500000;          /* limit for totient sieve  */
    const size_t TARGET = 50000;          /* number of sequence values to cache */

    /* 1. compute φ(i) for 1 ≤ i ≤ LIMIT.  uint32_t suffices.            */
    uint32_t *phi = (uint32_t *)malloc(sizeof *phi * (LIMIT + 1));
    if (!phi) return;
    for (idx_t i = 0; i <= LIMIT; ++i) phi[i] = (uint32_t)i;

    for (idx_t p = 2; p <= LIMIT; ++p)
    {
        if (phi[p] == (uint32_t)p)            /* p is prime             */
        {
            for (idx_t j = p; j <= LIMIT; j += p)
                phi[j] -= phi[j] / (uint32_t)p;
        }
    }

    /* 2. build a bitset of all totient values.                        */
    size_t bits_len = (LIMIT >> 6) + 1;      /* enough to hold value LIMIT-1 */
    uint64_t *present = (uint64_t *)calloc(bits_len, sizeof *present);
    if (!present) { free(phi); return; }

    for (idx_t i = 1; i <= LIMIT; ++i)
    {
        uint32_t v = phi[i];
        present[(size_t)(v >> 6)] |= 1ULL << (v & 63);
    }

    /* 3. generate the missing odd numbers (and 1).                     */
    seq_cache = (seq_t *)malloc(sizeof *seq_cache * TARGET);
    if (!seq_cache) { free(phi); free(present); return; }

    seq_len = 0;
    for (idx_t k = 1; k <= LIMIT && seq_len < TARGET; ++k)
    {
        if (k == 1)
        {
            seq_cache[seq_len++] = (seq_t)1;
        }
        else if ((k & 1U) && !bit_is_set(present, (uint32_t)(k - 1)))
        {
            seq_cache[seq_len++] = (seq_t)k;
        }
        /* even k > 2 are always representable – skip them            */
    }

    /* clean up temporary data */
    free(phi);
    free(present);
    init_done = true;
}

/* ----- public function: a(n) ------------------------------------------ */
seq_t a(idx_t n)
{
    if (!init_done) init_sequence();
    /* n ≥ 1; seq_cache[0] holds a(1).  n might be larger than cached
       terms for very large n, but the pre‑computed size TARGET is
       sufficient for sequences of practical size (≥10 000).            */
    return seq_cache[(size_t)(n - 1)];
}
