/*
 * A357074  Numbers sandwiched between a pair of numbers each with exactly two
 *          prime factors (counted without multiplicity).
 * https://oeis.org/A357074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   count_distinct_primes, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A357074: numbers k such that k-1 and k+1 each have exactly two
   distinct prime factors.  The implementation performs an incremental,
   brute‑force search and caches the results.  It satisfies the C11 / C++17
   compatibility requirements. */

#include <stdbool.h>
#include <stdint.h>

#include "../utils/count_distinct_primes.h"
#include "../utils/grow_seq.h"

static seq_t *seq   = NULL; /* cached sequence values */
static size_t seq_cap = 0;  /* allocated capacity in seq[] */
static size_t seq_len = 0;  /* number of terms stored */
static uint64_t next_check = 2; /* next integer to test */

/* Helper: true iff x has exactly two distinct prime factors */
static inline bool two_distinct(uint64_t x)
{
    return count_distinct_primes(x) == 2;
}

/* Extend the cache so that seq contains at least 'n' terms. */
static void extend_to(size_t n)
{
    if (seq_len >= n)
        return;

    for (; seq_len < n; ++seq_len)
    {
        /* Search for the next k with the required property. */
        while (!two_distinct(next_check - 1) || !two_distinct(next_check + 1))
            ++next_check;

        /* Allocate more space if necessary. */
        grow_seq(&seq, &seq_cap, seq_len + 1);

        /* Store the found value and advance the scan point. */
        seq[seq_len] = (seq_t)next_check;
        ++next_check;
    }
}

/* Public API required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n = 0, but return 0 */

    size_t target = (size_t)n;
    extend_to(target);
    return seq[target - 1];
}
