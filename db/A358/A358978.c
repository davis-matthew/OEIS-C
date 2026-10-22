/*
 * A358978  Numbers that are coprime to the number of terms in their
 *          Zeckendorf representation (A007895).
 * https://oeis.org/A358978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th positive integer k such that
   gcd(k, A007895(k)) = 1, where A007895(k) is the number of terms
   in the Zeckendorf representation of k. */

#include <stdlib.h>
#include "../utils/gcd.h"

/* Global buffer holding Fibonacci numbers used in Zeckendorf
   representations.  fib[0] = 1, fib[1] = 2. */
static seq_t *fib = NULL;
static size_t fib_size = 0; /* number of Fibonacci numbers stored */
static size_t fib_cap  = 0; /* allocated capacity */

/* Ensure the Fibonacci buffer has space for up to 'new_cap' elements. */
static void ensure_fib_cap(size_t new_cap) {
    if (new_cap <= fib_cap) return;
    size_t new_capacity = fib_cap ? fib_cap * 2 : 4;
    while (new_capacity < new_cap) new_capacity *= 2;
    seq_t *tmp = (seq_t*)realloc(fib, new_capacity * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failure */
    fib = tmp;
    fib_cap = new_capacity;
}

/* Grow the Fibonacci buffer until it contains a value >= 'limit'. */
static void ensure_fib_up_to(seq_t limit) {
    /* Initialise if empty. */
    if (fib_size == 0) {
        ensure_fib_cap(2);
        fib[0] = (seq_t)1;
        fib[1] = (seq_t)2;
        fib_size = 2;
    }
    /* Extend until the last element is >= limit. */
    while (fib[fib_size - 1] < limit) {
        seq_t next = fib[fib_size - 1] + fib[fib_size - 2];
        ensure_fib_cap(fib_size + 1);
        fib[fib_size++] = next;
    }
}

/* Return the n-th term of A358978. */
seq_t a(idx_t n) {
    idx_t found = 0;
    idx_t k = 1;
    seq_t result = 0;

    while (found < n) {
        seq_t kseq = (seq_t)k;
        /* Make sure we have enough Fibonacci numbers. */
        ensure_fib_up_to(kseq);

        /* Compute the Zeckendorf decomposition of kseq. */
        seq_t remaining = kseq;
        seq_t term_count = 0;

        while (remaining > 0) {
            /* Binary search for largest fib <= remaining. */
            idx_t lo = 0, hi = fib_size - 1, pos = 0;
            while (lo <= hi) {
                idx_t mid = lo + (hi - lo) / 2;
                if (fib[mid] <= remaining) {
                    pos = mid;
                    lo = mid + 1;
                } else {
                    if (mid == 0) break;
                    hi = mid - 1;
                }
            }
            remaining = remaining - fib[pos];
            term_count = term_count + 1;
        }

        /* Check gcd condition. */
        seq_t g = gcd(kseq, term_count);
        if (g == (seq_t)1) {
            found++;
            result = kseq;
        }
        k++;
    }
    return result;
}
