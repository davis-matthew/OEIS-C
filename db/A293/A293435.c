/*
 * A293435  a(n) is the number of the proper divisors of n that are Fibonacci
 *          numbers (A000045).
 * https://oeis.org/A293435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of proper divisors of n that are Fibonacci
 *            numbers. Implementation: pre‑compute unique Fibonacci numbers up
 *            to n‑1 and count those that divide n. The list of Fibonacci
 *            numbers grows only logarithmically in n, so the routine is O(log
 *            n) per call
 * Range:     u8 n <= 4359, u16 n <= 8584, u32 n <= 8584, u64 n <= 8584,
 *            u128 n <= 8584, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *fib_cache = NULL;   /* array of Fibonacci numbers (>0, unique) */
static size_t fib_count = 0;      /* number of elements currently stored */
static size_t fib_cap   = 0;      /* current capacity of fib_cache */

/* Ensure that fib_cache contains all unique Fibonacci numbers ≤ limit. */
static void ensure_fibs(idx_t limit)
{
    /* Initialisation: store 1 and 2 (unique positive Fibonacci values). */
    if (fib_count == 0) {
        fib_cap = 8;
        fib_cache = (seq_t*)malloc(fib_cap * sizeof(seq_t));
        if (!fib_cache) return; /* OOM – callers will receive 0 */
        fib_cache[0] = 1;
        fib_cache[1] = 2;
        fib_count   = 2;
    }

    /* Generate further terms while the last term ≤ limit. */
    while (fib_count >= 2 && fib_cache[fib_count - 1] <= limit) {
        /* Expand the array if necessary. */
        if (fib_count == fib_cap) {
            fib_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(fib_cache, fib_cap * sizeof(seq_t));
            if (!tmp) return; /* OOM */
            fib_cache = tmp;
        }

        /* Next Fibonacci number (unique – no duplicate 1). */
        seq_t next = fib_cache[fib_count - 1] + fib_cache[fib_count - 2];
        if (next < fib_cache[fib_count - 1]) return;   /* overflow check */
        if (next > limit) break;                       /* stop if beyond limit */

        fib_cache[fib_count++] = next;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* safety for non‑positive n */

    /* Prepare Fibonacci numbers < n (proper divisors exclude n itself). */
    ensure_fibs(n - 1);

    seq_t count = 0;
    for (size_t i = 0; i < fib_count && fib_cache[i] < (seq_t)n; ++i) {
        if (n % fib_cache[i] == 0) ++count;
    }
    return count;
}
