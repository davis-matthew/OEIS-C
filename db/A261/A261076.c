/*
 * A261076  The infinite trunk of Zeckendorf (Fibonacci) beanstalk, with
 *          reversed subsections.
 * https://oeis.org/A261076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 7624, u32 n <= 9040, u64 n <= 9040,
 *            u128 n <= 9040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261076: 
   The infinite trunk of Zeckendorf (Fibonacci) beanstalk,
   defined by successive rows starting at k = F(n+2)-1,
   subtracting the number of Zeckendorf ones until a value is
   repeated.  Implements the construction directly. */
#include <stdlib.h>
#include <stdint.h>

/* helper: count number of 1‑fibits (terms) in the Zeckendorf expansion of x */
static unsigned count_ones(seq_t x, seq_t *fib, size_t fib_len)
{
    unsigned cnt = 0;
    while (x > (seq_t)0) {
        /* find largest fib <= x */
        size_t i = fib_len - 1;
        while (fib[i] > x) --i;
        x -= fib[i];
        ++cnt;
    }
    return cnt;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    /* allocate storage for sequence values; 10000 terms + margin */
    const size_t MAX_STORAGE = 20000;
    seq_t seq[MAX_STORAGE];
    size_t seq_len = 3;          /* 0,1,2 already stored */
    seq[0] = 0; seq[1] = 1; seq[2] = 2;

    /* Fibonacci table: fib[0]=F(1)=1, fib[1]=F(2)=1 */
    const size_t FIB_MAX = 2000;
    seq_t fib[FIB_MAX];
    fib[0] = (seq_t)1;
    fib[1] = (seq_t)1;
    size_t fib_len = 2;

    /* current row index: the index of the first term of the current row */
    size_t row_index = 3;

    while (seq_len <= n) {
        /* ensure fibonacci numbers up to F(row_index+2) exist */
        while (row_index + 1 >= fib_len && fib_len < FIB_MAX) {
            fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
            ++fib_len;
        }

        /* start value for this row */
        seq_t current = fib[row_index + 1] - (seq_t)1;

        /* generate terms of this row */
        while (1) {
            seq[seq_len++] = current;
            if (seq_len - 1 == n) return current;   /* reached requested n */

            unsigned ones = count_ones(current, fib, fib_len);
            seq_t next = current - (seq_t)ones;

            /* check if next already appears in the sequence */
            int found = 0;
            for (size_t i = 0; i < seq_len - 1; ++i)
                if (seq[i] == next) { found = 1; break; }

            if (found) break;          /* row ends */
            current = next;            /* continue this row */
        }

        ++row_index;                   /* move to next row */
    }

    /* unreachable, but keeps compiler happy */
    return seq[n];
}
