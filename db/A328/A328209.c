/*
 * A328209  Numbers m such that m and m+1 are consecutive Zeckendorf-Niven
 *          numbers (A328208).
 * https://oeis.org/A328209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1172, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Zeckendorf‑Niven consecutive pairs – search algorithm.
   Computes the n‑th integer m such that both m and m+1 are Zeckendorf‑Niven.
   The algorithm scans m from 1 upwards, tests the Zeckendorf‑Niven property
   for m and m+1, and stores the successful m’s in a memoised array. */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

/* ---------- Fibonacci cache for Zeckendorf digit sums ---------- */
#define MAX_FIBS 2000

static seq_t fibs[MAX_FIBS];
static size_t fib_count = 0;

/* Initialise or extend the Fibonacci table such that it contains all
   Fibonacci numbers up to at least 'limit'.  Overflow is detected by
   checking for wrap‑around of the addition. */
static void ensure_fibs_up_to(seq_t limit)
{
    if (fib_count == 0) {
        fibs[0] = (seq_t)1;
        fibs[1] = (seq_t)2;
        fib_count = 2;
    }
    while (fib_count < MAX_FIBS) {
        seq_t prev = fibs[fib_count - 2];
        seq_t cur  = fibs[fib_count - 1];
        seq_t next = prev + cur;
        /* stop if addition wrapped */
        if (next < cur || next < prev) break;
        fibs[fib_count++] = next;
        if (next >= limit) break;
    }
}

/* Greedy Zeckendorf decomposition: return the number of Fibonacci
   summands required to represent 'n'.  The result fits in seq_t. */
static seq_t zeckendorf_sum(seq_t n)
{
    seq_t sum = (seq_t)0;
    if (n == (seq_t)0) return sum;

    ensure_fibs_up_to(n);

    seq_t rem = n;
    while (rem > (seq_t)0) {
        int lo = 0;
        int hi = (int)fib_count - 1;
        /* largest Fibonacci ≤ rem */
        while (lo < hi) {
            int mid = (lo + hi + 1) >> 1;
            if (fibs[mid] <= rem)
                lo = mid;
            else
                hi = mid - 1;
        }
        rem = rem - fibs[lo];
        sum = sum + (seq_t)1;
    }
    return sum;
}

/* True iff 'n' is a Zeckendorf‑Niven number: its Zeckendorf digit sum
   divides it.  For n==0 it returns false. */
static bool zeckendorf_niven(seq_t n)
{
    seq_t sum = zeckendorf_sum(n);
    if (sum == (seq_t)0) return false;
    return n % sum == (seq_t)0;
}

/* ---------- Memoised list of valid m’s ---------- */
#define MAX_TERMS 20000   /* capacity for memoisation */
static seq_t terms[MAX_TERMS];
static size_t terms_len = 0;
static seq_t last_scanned_m = 0;

/* Return the nth element (n>=1) of the sequence.  Subsequent calls
   for smaller n are served from the memoised array. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n=0 */

    /* Generate terms up to the requested index if necessary */
    while (terms_len < (size_t)n) {
        seq_t m = last_scanned_m + (seq_t)1;
        if (zeckendorf_niven(m) && zeckendorf_niven(m + (seq_t)1)) {
            if (terms_len < MAX_TERMS)
                terms[terms_len++] = m;   /* store found m */
            /* If terms_len reaches MAX_TERMS we stop; this should not
               happen for n ≤ 10000 in the test data. */
        }
        last_scanned_m = m;
    }

    return terms[n - 1];
}
