/*
 * A395638  a(n) = number of triples (x, y, z) such that 2*x^2 + y*z = n,
 *          where x, y, z are positive Fibonacci numbers.
 * https://oeis.org/A395638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (x, y, z) of positive Fibonacci
 *            numbers such that 2*x^2 + y*z = n
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Fibonacci numbers are generated once and reused.
   We keep only distinct positive Fibonacci values:
   1, 2, 3, 5, 8, ...
   The array is sized generously; for n up to 10000 only the first 20
   entries are required.                                       */
static seq_t fib[100];
static idx_t fib_cnt = 0;   /* number of Fibonacci values stored */
static idx_t fib_limit = 0; /* largest n for which fib has been generated */

/* Ensure that fib[0..fib_cnt-1] contains all distinct Fibonacci numbers
   <= the specified limit. */
static void ensure_fib(idx_t limit)
{
    if (limit <= fib_limit) return;

    /* initialise if the table is empty */
    if (fib_cnt == 0) {
        fib[0] = 1;            /* first distinct Fibonacci number */
        fib[1] = 2;            /* second distinct Fibonacci number */
        fib_cnt = 2;
    }

    /* extend the table up to 'limit' */
    while (fib_cnt < 100) {
        seq_t next = fib[fib_cnt-1] + fib[fib_cnt-2];
        if (next > limit) break;
        fib[fib_cnt++] = next;
    }
    fib_limit = limit;
}

/* Binary search for a value in the generated Fibonacci table. */
static int is_fib(seq_t value)
{
    idx_t lo = 0, hi = fib_cnt;
    /* hi is exclusive */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (fib[mid] < value) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return (lo < fib_cnt && fib[lo] == value) ? 1 : 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* For n < 3 there are no solutions, but the loops handle it elegantly. */
    ensure_fib(n);          /* need Fibonacci numbers up to n */

    seq_t count = 0;

    /* Iterate over all possible x values */
    for (idx_t ix = 0; ix < fib_cnt; ++ix) {
        seq_t x = fib[ix];
        /* compute 2 * x^2, careful with intermediate size */
        seq_t two_x2 = x;
        two_x2 = two_x2 << 1;   /* multiply by 2 */
        two_x2 = two_x2 * x;    /* now 2 * x^2 */

        if (two_x2 > n) break; /* further x's are larger */

        seq_t rem = n - two_x2;

        /* Iterate over all possible y values */
        for (idx_t iy = 0; iy < fib_cnt; ++iy) {
            seq_t y = fib[iy];
            if (y > rem) break;
            if (rem % y != 0) continue;

            seq_t z = rem / y;
            if (!is_fib(z)) continue;

            /* Found a valid ordered triple (x, y, z) */
            count += 1;
        }
    }

    return count;
}
