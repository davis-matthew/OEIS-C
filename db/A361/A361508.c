/*
 * A361508  a(n) = smallest k such that Fibonacci(k) = n, or -1 if n is not a
 *          Fibonacci number.
 * https://oeis.org/A361508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = smallest k such that Fibonacci(k) = n, or -1 if n is not
 *            a Fibonacci number
 * Range:     i8 n <= 89, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* The Fibonacci numbers are stored in a static dynamic array.  */
static seq_t *fibs = NULL;
static size_t fibs_size = 0;  /* Number of Fibonacci numbers stored. */
static size_t fibs_cap  = 0;  /* Capacity of the array. */

/* Ensure that fibs has at least 'needed' entries.  Allocate or
   reallocate as necessary.  */
static void ensure_fib_capacity(size_t needed)
{
    if (needed <= fibs_cap)
        return;
    size_t new_cap = fibs_cap ? fibs_cap << 1 : 2;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(fibs, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* Out of memory: abort, failing to compute the sequence.  */
        abort();
    }
    fibs = tmp;
    fibs_cap = new_cap;
}

/* Compute Fibonacci numbers until the last term is >= target, or until
   an overflow stops further growth.  */
static void extend_fibs_up_to(seq_t target)
{
    if (fibs_size == 0) {
        /* Initialize with F(0)=0, F(1)=1. */
        ensure_fib_capacity(2);
        fibs[0] = (seq_t)0;
        fibs[1] = (seq_t)1;
        fibs_size = 2;
    }

    while (fibs_size > 0 && fibs[fibs_size - 1] < target) {
        /* Compute next Fibonacci number. */
        seq_t next = fibs[fibs_size - 1] + fibs[fibs_size - 2];
        /* Detect overflow: if next < previous, the addition wrapped. */
        if (next < fibs[fibs_size - 1])
            break;
        ensure_fib_capacity(fibs_size + 1);
        fibs[fibs_size++] = next;
    }
}

/* Binary search for the target in the Fibonacci array.  Return the
   index if found, or -1 if not found.  */
static seq_t find_fib_index(seq_t target)
{
    size_t lo = 0, hi = fibs_size;  /* [lo, hi) */
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (fibs[mid] < target)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < fibs_size && fibs[lo] == target)
        return (seq_t)lo;
    return (seq_t)-1;
}

/* Public entry point for the sequence.  */
seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    extend_fibs_up_to(target);
    return find_fib_index(target);
}
