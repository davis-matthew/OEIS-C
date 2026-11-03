/*
 * A353656  Number of terms in the Lucas-Fibonacci representation of n.
 * https://oeis.org/A353656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 339, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9370 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses greedy alternating Lucas and Fibonacci representation. */
#include <stddef.h>
#include <stdbool.h>

/* Return the largest element in arr[0..sz) that is <= rem. */
static seq_t max_le(const seq_t *arr, size_t sz, seq_t rem)
{
    seq_t best = arr[0];
    for (size_t i = 1; i < sz; ++i) {
        if (arr[i] <= rem && arr[i] > best)
            best = arr[i];
    }
    return best;
}

static seq_t lucas[64];
static size_t lucas_cnt = 0;
static seq_t fib[64];
static size_t fib_cnt = 0;
static idx_t precomp_limit = 0;

/* Precompute Lucas and Fibonacci numbers up to at least `limit`. */
static void precompute(idx_t limit)
{
    if (limit <= precomp_limit)
        return;

    /* Lucas numbers: 1, 3, 4, 7, 11, ...  (exclude 2). */
    lucas_cnt = 0;
    seq_t a = 1, b = 3;
    while (a <= (seq_t)limit) {
        lucas[lucas_cnt++] = a;
        seq_t c = a + b;
        a = b;
        b = c;
    }

    /* Fibonacci numbers: 1, 1, 2, 3, 5, ... */
    fib_cnt = 0;
    seq_t f0 = 0, f1 = 1;
    while (f1 <= (seq_t)limit) {
        fib[fib_cnt++] = f1;
        seq_t f2 = f0 + f1;
        f0 = f1;
        f1 = f2;
    }

    precomp_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset 1, not used */

    precompute(n);

    seq_t rem = (seq_t)n;
    seq_t count = 0;
    bool use_lucas = true;

    while (rem > (seq_t)0) {
        seq_t term = use_lucas
            ? max_le(lucas, lucas_cnt, rem)
            : max_le(fib,   fib_cnt,   rem);
        rem -= term;
        ++count;
        use_lucas = !use_lucas;
    }
    return count;
}
