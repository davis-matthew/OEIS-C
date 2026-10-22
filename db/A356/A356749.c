/*
 * A356749  a(n) is the number of trailing 1's in the dual Zeckendorf
 *          representation of n (A104326).
 * https://oeis.org/A356749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   number of trailing 1's in the dual Zeckendorf representation of
 *            n (A104326)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t fib[95];
static idx_t cum[95];
static int fibCount = 0;
static bool initDone = false;

/* generate Fibonacci numbers F1=1, F2=2 and finite prefix sums */
static void init_fibs(void)
{
    if (initDone) return;
    initDone = true;

    fib[1] = 1;
    fib[2] = 2;
    fibCount = 2;
    for (int i = 3; i < 95; ++i) {
        if (fib[i-1] > UINT64_MAX - fib[i-2]) break; /* overflow guard */
        fib[i] = fib[i-1] + fib[i-2];
        fibCount = i;
    }
    cum[0] = 0;
    for (int i = 1; i <= fibCount; ++i) {
        cum[i] = cum[i-1] + fib[i];
    }
}

/* check whether rem can be represented using distinct Fibonacci numbers
   whose indices are >= minIdx (Zeckendorf greedy from >minIdx) */
static bool canRepresent(idx_t rem, int minIdx)
{
    if (rem == 0) return true;
    int idx = fibCount;          /* start from largest Fibonacci */
    while (rem > 0) {
        while (idx >= minIdx && fib[idx] > rem) idx--;   /* find usable fib */
        if (idx < minIdx) return false;                   /* impossible */
        rem -= fib[idx];
        idx--;                                            /* cannot reuse same fib */
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    init_fibs();

    /* find maximal t such that sum_{i=1}^t fib[i] <= n  */
    int tmax = 0;
    idx_t sum = 0;
    for (int i = 1; i <= fibCount; ++i) {
        if (sum + fib[i] <= n) {
            sum += fib[i];
            tmax = i;
        } else {
            break;
        }
    }

    /* try t from tmax downwards, first feasible t is the answer */
    for (int t = tmax; t >= 0; --t) {
        idx_t sum_t = cum[t];
        if (sum_t > n) continue;
        idx_t rem = n - sum_t;
        if (canRepresent(rem, t + 2))
            return (seq_t)t;
    }
    return (seq_t)0;   /* safety, should never happen */
}
