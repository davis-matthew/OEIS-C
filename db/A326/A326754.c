/*
 * A326754  BII-numbers of set-systems covering an initial interval of
 *          positive integers.
 * https://oeis.org/A326754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9462 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nth BII-number of set‑systems covering an initial interval of positive integers.
 * The function iterates over successive non‑negative integers, tests each for
 * the covering property, and returns the *n*th one (a(1)=0). */

#include <stdbool.h>
#include <stdint.h>

/* Test whether the integer n generates a set‑system that covers
   an initial contiguous interval of positive integers.  The set‑system
   is defined from n by taking the binary indices of each binary
   index of n.  The test works with a 64‑bit mask because no index
   needed for the given limits exceeds 64. */
static bool covers_initial_interval(idx_t n)
{
    uint64_t union_mask = 0;   /* union of all binary indices of the indices of n */
    unsigned int max_index = 0;/* largest index that appears */

    idx_t m = n;
    unsigned int bit_pos = 1;   /* positions of set bits in n (1‑based) */
    while (m)
    {
        if (m & 1UL)
        {
            /* For the bit position 'bit_pos', compute its binary indices. */
            idx_t t = bit_pos;
            unsigned int inner_pos = 1;
            uint64_t set_mask = 0;
            unsigned int local_max = 0;
            while (t)
            {
                if (t & 1UL)
                {
                    set_mask |= (1ULL << (inner_pos - 1));
                    if (inner_pos > local_max)
                        local_max = inner_pos;
                }
                t >>= 1;
                inner_pos++;
            }
            union_mask |= set_mask;
            if (local_max > max_index)
                max_index = local_max;
        }
        m >>= 1;
        bit_pos++;
    }

    if (max_index == 0)
        return true;           /* empty set covers {} */

    uint64_t full_mask = (max_index >= 64) ? ~0ULL : ((1ULL << max_index) - 1);
    return union_mask == full_mask;
}

/* Return the nth BII‑number (1‑based indexing). */
seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t cur   = 0;          /* test non‑negative integers in order */
    while (found < n)
    {
        if (covers_initial_interval(cur))
        {
            ++found;
            if (found == n)
                return (seq_t)cur;
        }
        ++cur;
    }
    /* Unreachable, but silence the compiler. */
    return 0;
}
