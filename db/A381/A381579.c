/*
 * A381579  The Chung-Graham representation of n: representation of n in base
 *          of even-indexed Fibonacci numbers.
 * https://oeis.org/A381579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function computes OEIS sequence A381579 – the Chung‑Graham
    representation of n in a base of even‑indexed Fibonacci numbers.
    The digits are determined greedily, each digit ∈ {0,1,2} and no
    two '2' digits occur without a zero between them.  The result is
    the ordinary decimal integer whose decimal digits are those digits. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Build even‑indexed Fibonacci numbers ≤ n (ascending).  The
       sequence grows very slowly, so a fixed 60‑element array is ample
       for all n that fit in idx_t (uint64_t). */
    uint64_t fib[60];
    size_t   fib_cnt = 0;
    uint64_t f0 = 0, f1 = 1;
    size_t   idx = 1;   /* Fibonacci index (1‑based) */

    while (f1 <= n) {
        if ((idx & 1) == 0) {          /* even index -> store */
            if (fib_cnt < 60) fib[fib_cnt++] = f1;
        }
        /* Stop before unsigned overflow. */
        if (f0 > UINT64_MAX - f1) break;
        uint64_t f2 = f0 + f1;
        f0 = f1;
        f1 = f2;
        ++idx;
    }

    /* Greedy construction of the representation.  Process from the
       largest Fibonacci down to the smallest; build the decimal
       number digit by digit. */
    seq_t   result   = 0;
    idx_t   rest     = n;

    for (size_t i = fib_cnt; i-- > 0; ) {
        idx_t d = rest / fib[i];
        if (d > 2) d = 2;                 /* digit cannot exceed 2 */
        rest -= d * fib[i];
        result = result * 10 + (seq_t)d;  /* append the digit */
    }

    return result;
}
