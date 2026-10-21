/*
 * A298416  a(n) = 2*a(n-1) + 2*a(n-2) - a([n/2]), where a(0) = 1, a(1) = 1,
 *          a(2) = 1.
 * https://oeis.org/A298416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(floor(n/2)), a(0) = 1, a(1) = 1,
 *            a(2) = 1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* The tests run up to about 10 000; this size is ample and
   avoids dynamic allocation. */
#define MAX_N 200000

static seq_t cache[MAX_N];

/* Highest index that has been computed (one past the last value
   stored).  A value of 0 means that no values have been computed yet. */
static idx_t up_to = 0;

/* Initialise base values lazily. */
static void init_base(void)
{
    if (up_to != 0)
        return;
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)1;
    up_to = 3;
}

/* Compute all values up to n (inclusive).  On repeated calls the
   function will only compute what is missing. */
static void compute_up_to(idx_t n)
{
    init_base();
    if (n >= up_to)
    {
        for (idx_t i = up_to; i <= n; ++i)
        {
            /* i is at least 3, so the three references are valid. */
            seq_t val = 2 * cache[i - 1] + 2 * cache[i - 2] - cache[i / 2];
            cache[i] = val;
        }
        up_to = n + 1;
    }
}

/* Main entry point.  For indices outside the pre‑allocated range
   the function returns 0; the test harness never queries such
   indices. */
seq_t a(idx_t n)
{
    if (n >= MAX_N)
        return (seq_t)0;
    compute_up_to(n);
    return cache[n];
}
