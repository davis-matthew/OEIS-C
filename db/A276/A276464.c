/*
 * A276464  Fibbinary Fibonacci numbers.
 * https://oeis.org/A276464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th Fibonacci number that is also a Fibbinary number
   (i.e., its binary representation contains no consecutive 1s).  Duplicates
   from the Fibonacci sequence (e.g., 1 appears twice) are counted only once. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initialise Fibonacci iteration: F(0)=0, F(1)=1 */
    seq_t f_prev = 0;
    seq_t f_curr = 1;

    /* Keep track of the last unique Fibonacci value seen to avoid counting
       duplicates such as the two initial 1's. */
    seq_t prev_unique = (seq_t)-1;

    idx_t count = 0;   /* number of fibbinary Fibonacci values found */

    for (;;)
    {
        /* If this Fibonacci number is unique and is Fibbinary, count it. */
        if (f_prev != prev_unique && ((f_prev & (f_prev >> 1)) == 0))
        {
            ++count;
            if (count == n)
                return f_prev;
        }

        /* Prepare for next Fibonacci number. */
        prev_unique = f_prev;
        seq_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;

        /* Detect overflow on 64‑bit builds and terminate gracefully. */
        if (sizeof(seq_t) == sizeof(unsigned long long))
            if (f_curr < f_prev)      /* addition overflowed */
                break;
    }

    /* If the sequence has fewer terms than requested, return 0. */
    return 0;
}
