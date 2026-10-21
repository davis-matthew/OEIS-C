/*
 * A250047  Numbers n such that m = floor(n/7) is not coprime to n and, if
 *          nonzero, m is also a term of the sequence.
 * https://oeis.org/A250047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 3909, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence A250047 is the set of integers n such that
   m = floor(n/7) is not coprime to n, and if m ≠ 0 then m itself
   must also belong to the sequence (RTNC(7)).
   This function computes the nth term (n≥1) by iteratively
   generating terms, storing them in a static array, and using
   binary search to verify membership of m. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/grow_seq.h"

/* Binary search: returns true if value is present in sorted array arr[0..len-1] */
static bool contains(const seq_t *arr, size_t len, seq_t value)
{
    size_t lo = 0, hi = len;
    while (lo < hi)
    {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] < value)
            lo = mid + 1;
        else if (arr[mid] == value)
            return true;
        else
            hi = mid;
    }
    return false;
}

static seq_t *seq = NULL;      /* dynamic array of generated terms */
static size_t seq_len = 0;      /* number of terms stored */
static size_t seq_cap = 0;      /* allocated capacity of seq */
static idx_t next_candidate = 2;   /* next integer to test for inclusion */

/* Return nth term of the sequence (n≥1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* not used, sequence is 1‑based */

    /* Grow the sequence until we have at least n terms. */
    while (seq_len < (size_t)n)
    {
        seq_t cand = (seq_t)next_candidate;
        seq_t m = cand / (seq_t)7;  /* floor(cand/7) */

        /* Must not be coprime to m. */
        if (gcd(cand, m) != (seq_t)1)
        {
            /* If m > 0, it must already be in the sequence. */
            if (m == (seq_t)0 || contains(seq, seq_len, m))
            {
                /* Append cand to the sequence. */
                grow_seq(&seq, &seq_cap, seq_len + 1);
                seq[seq_len++] = cand;
            }
        }
        ++next_candidate;
    }

    return seq[n - 1];
}
