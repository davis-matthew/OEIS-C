/*
 * A305820  Filter sequence for a(Fibonacci numbers > 1) = constant sequences.
 * https://oeis.org/A305820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 0, * a(1) = 1, * for n > 1, * a(n) = 2 if n is a
 *            Fibonacci number > 1; * otherwise a(n) = 2 + n - A072649(n), *
 *            where A072649(n) counts distinct Fibonacci numbers > 0 that are
 *            ≤ n
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *fib = NULL;
static size_t fib_size  = 0;
static size_t fib_cap   = 0;

/* Ensure Fibonacci table contains all values up to and beyond limit. */
static void ensure_fibs_up_to(seq_t limit)
{
    if (fib_cap == 0) {
        fib_cap  = 16;
        fib      = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        if (!fib) return;          /* allocation failed – subsequent calls will fail silently */
        fib[0]   = 0;
        fib[1]   = 1;
        fib_size = 2;
    }

    /* Generate Fibonacci numbers until the last one exceeds limit. */
    while (fib[fib_size - 1] <= limit) {
        seq_t next = fib[fib_size - 1] + fib[fib_size - 2];
        if (fib_size >= fib_cap) {
            fib_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(fib, fib_cap * sizeof(seq_t));
            if (!tmp) return;      /* allocation failed – leave the array as is */
            fib = tmp;
        }
        fib[fib_size] = next;
        ++fib_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t n_seq = (seq_t)n;
    ensure_fibs_up_to(n_seq);

    /* Binary search for the largest Fibonacci number ≤ n_seq. */
    size_t lo = 0, hi = fib_size;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (fib[mid] <= n_seq)
            lo = mid + 1;
        else
            hi = mid;
    }
    size_t idx = lo - 1;            /* idx is the index of the largest fib ≤ n_seq */

    /* If n_seq is a Fibonacci number greater than 1, return 2. */
    if (fib[idx] == n_seq && n_seq > (seq_t)1)
        return (seq_t)2;

    /* Non‑Fibonacci case: a(n) = 2 + n - (#distinct fib >0 ≤ n). */
    /* Count of distinct fib >0 ≤ n is (idx - 1), valid for n > 1. */
    seq_t count = (seq_t)(idx - 1);
    return (seq_t)2 + n_seq - count;
}
