/*
 * A242057  Even numbers n for which lpf_3(n-1) < lpf_3(n-3), where lpf_3(n) =
 *          lpf(n/3^t) (cf. A020639) such that 3^t (t>=0) is the maximal power
 *          of 3 which divides n.
 * https://oeis.org/A242057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _lpf_3_uint64, _lpf_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation based on the definition:
   a(n) = smallest even integer m that satisfies
   lpf_3(m-1) < lpf_3(m-3).
   lpf_3(x) = lpf(x / 3^t), where 3^t is the maximal power of 3 dividing x. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_lpf_3_uint64.h"
#include "../utils/_lpf_simple.h"

/* Memoised storage of the sequence. */
static seq_t *seq      = NULL;   /* array of terms (0‑based) */
static size_t seq_len  = 0;      /* number of terms stored */
static size_t seq_cap  = 0;      /* capacity of the array */
static uint64_t next_candidate = 4ULL; /* next even number to test */

/* Ensure capacity for at least need elements.  Simple doubling strategy. */
static void _ensure_capacity(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap == 0 ? 128 : seq_cap;
    while (new_cap < need) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Main accessor – generates terms on demand, memoised. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 0 – harmless */

    /* Extend the sequence until we have at least n terms. */
    while (seq_len < (size_t)n)
    {
        /* Find next even number satisfying the condition. */
        while (1)
        {
            uint64_t m = next_candidate;      /* current candidate (even) */
            uint64_t lpf1 = _lpf_3_uint64(m - 1ULL);
            uint64_t lpf3 = _lpf_3_uint64(m - 3ULL);
            if (lpf1 < lpf3)           /* condition satisfied */
            {
                _ensure_capacity(seq_len + 1);
                seq[seq_len] = (seq_t)m;  /* store as seq_t */
                seq_len++;
                /* Advance to the next even candidate beyond current m. */
                next_candidate = m + 2ULL;
                break;
            }
            next_candidate += 2ULL; /* try next even number */
        }
    }

    return seq[n - 1U];   /* seq is 0‑indexed, n is 1‑based */
}
