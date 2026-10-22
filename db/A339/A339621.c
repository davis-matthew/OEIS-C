/*
 * A339621  Sum of Fibonacci divisors of n^2 + 1.
 * https://oeis.org/A339621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of Fibonacci numbers that divide n^2 + 1, counting 1
 *            once
 * Range:     u8 n <= 16, u16 n <= 618, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t fibs[200];          /* Fibonacci numbers starting 1, 2, 3, 5, ...  */
static size_t fib_cnt = 0;       /* Number of Fibonacci values stored */
static int fibs_initialized = 0;

/* Initialize the Fibonacci table once, up to the maximum representable value of seq_t. */
static void init_fibs(void)
{
    if (fibs_initialized)
        return;
    fibs[0] = (seq_t)1;
    fibs[1] = (seq_t)2;
    fib_cnt = 2;
    while (1) {
        seq_t next = fibs[fib_cnt - 1] + fibs[fib_cnt - 2];
        /* Stop if addition overflows: wrapped arithmetic makes next < previous. */
        if (next < fibs[fib_cnt - 1])
            break;
        fibs[fib_cnt] = next;
        fib_cnt++;
        if (fib_cnt >= sizeof(fibs)/sizeof(fibs[0]))
            break; /* safety in case of unexpected large seq_t */
    }
    fibs_initialized = 1;
}

seq_t a(idx_t n)
{
    init_fibs();                       /* ensure fibs ready */

    seq_t m = (seq_t)n * (seq_t)n + (seq_t)1; /* n^2 + 1 in seq_t */
    seq_t sum = (seq_t)0;

    for (size_t i = 0; i < fib_cnt; ++i) {
        if (fibs[i] > m)
            break;
        if (m % fibs[i] == (seq_t)0)
            sum += fibs[i];
    }
    return sum;
}
