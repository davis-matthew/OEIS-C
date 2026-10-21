/*
 * A296219  Solution of the complementary equation a(n) = a(0)*b(n-1) +
 *          a(1)*b(n-2), where a(0) = 1, a(1) = 3, b(0) = 2, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3, a(n) = a(0)*b(n‑1) + a(1)*b(n‑2) where a(n)
 *            and b(n) are increasing complementary sequences partitioning the
 *            positive integers. The implementation generates a and b
 *            iteratively, keeping track of the smallest unused number to
 *            supply the next b term. It runs in O(n) time and use...
 * Range:     u8 n <= 50, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
/* C++ builds: use std::uint64_t for idx_t when not defined elsewhere */
# ifndef idx_t
#  include <cstdint>
#  define idx_t std::uint64_t
# endif
#endif

/* Static storage for the sequences and auxiliary indices.  */
static seq_t a_vals[200000];
static seq_t b_vals[200000];
static size_t a_len  = 0;          /* number of a terms stored         */
static size_t b_len  = 0;          /* number of b terms stored         */
static size_t idx_a = 0;           /* scan index for a[]              */
static size_t idx_b = 0;           /* scan index for b[]              */
static seq_t next_unused = 1;      /* candidate for the next unused int */

/* Forward declaration */
static void grow_to(idx_t n);

/* Public function required by the problem. */
seq_t
a(idx_t n)
{
    grow_to(n);
    return a_vals[n];
}

/*======================================================================
    Internal functions
  ======================================================================*/

/* Ensure the cached sequences contain at least up to index n. */
static void
grow_to(idx_t n)
{
    /* Initialization on first call.  */
    if (a_len == 0) {
        /* Define the known starting terms.  */
        if (sizeof(a_vals) / sizeof(a_vals[0]) < 2 ||
            sizeof(b_vals) / sizeof(b_vals[0]) < 1) {
            /* Too small; program cannot proceed.  */
            abort();
        }
        a_vals[0] = (seq_t)1;
        a_vals[1] = (seq_t)3;
        a_len = 2;

        b_vals[0] = (seq_t)2;
        b_len = 1;

        idx_a = 0;
        idx_b = 0;
        next_unused = 1;               /* will be turned into 4 after first skip */
    }

    /* Grow until we have a[n].  */
    while (a_len <= n) {
        /* Make sure we have b[n‑1] and b[n‑2].  */
        while (b_len < a_len) {
            /* Find the next unused integer.  */
            while ( ((idx_a < a_len) && (a_vals[idx_a] == next_unused)) ||
                    ((idx_b < b_len) && (b_vals[idx_b] == next_unused)) ) {
                if ((idx_a < a_len) && (a_vals[idx_a] == next_unused))
                    ++idx_a;
                if ((idx_b < b_len) && (b_vals[idx_b] == next_unused))
                    ++idx_b;
                ++next_unused;
            }
            /* Register the found integer as b[b_len].  */
            b_vals[b_len] = next_unused;
            ++b_len;
            ++next_unused;               /* start searching after this value   */
        }

        /* Compute the next a value.  */
        seq_t val = a_vals[0] * b_vals[a_len-1] + a_vals[1] * b_vals[a_len-2];
        a_vals[a_len] = val;
        ++a_len;
    }
}
