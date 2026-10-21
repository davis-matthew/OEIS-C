/*
 * A247647  Binary numbers that begin and end with 1 and do not contain two
 *          adjacent zeros.
 * https://oeis.org/A247647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 143, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by constructing the binary string
   that begins and ends with 1 and contains no two adjacent zeros,
   then interpreting that string as a decimal number. */
#include <stddef.h>

#define MAX_FIB 70

/* fib[i] = number of length‑i binary strings that start and end with 1
   and have no adjacent zeros.  These counts form a Fibonacci sequence
   with fib[0]=0, fib[1]=1. */
static seq_t fib[MAX_FIB];
static size_t fib_len = 0;

/* Ensure fib array covers up to idx (inclusive). */
static void ensure_fib(size_t idx)
{
    while (fib_len <= idx) {
        fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
        ++fib_len;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)    /* offset is 1; 0 not a valid term */
        return (seq_t)0;

    /* Initialise fib[0] and fib[1] once. */
    if (fib_len == 0) {
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib_len = 2;
    }

    /* Find the length L such that the cumulative count up
       to length L-1 is < n ≤ cumulative count up to length L. */
    idx_t L = 1;
    seq_t cum = (seq_t)0;
    seq_t target = (seq_t)n;

    for (;;) {
        ensure_fib(L);
        seq_t countL = fib[L];          /* number of strings of length L */
        if (cum + countL >= target)    /* found required length */
            break;
        cum += countL;
        ++L;
    }

    /* Rank k (1‑based) within strings of length L. */
    seq_t k = target - cum;
    if (L == 1)          /* only the string "1" */
        return (seq_t)1;

    /* Length of the middle part (positions 2 … L‑1). */
    idx_t M = L - 2;

    /* Ensure fib array has enough entries for the construction. */
    ensure_fib(M + 2);

    /* Construct the number as a decimal string composed of 0/1 bits. */
    seq_t ans = (seq_t)1;          /* leading 1 */
    int prev = 1;                  /* previous bit (before middle) */

    for (idx_t pos = 0; pos < M; ++pos) {
        idx_t rem = M - pos;                /* remaining positions including current */
        seq_t cnt0 = 0;
        if (prev == 1)
            cnt0 = fib[rem];                /* placing 0 is allowed only after a 1 */

        seq_t cnt1 = fib[rem + 1];          /* number of completions with a 1 at this place */

        if (cnt0 >= k) {                    /* choose bit 0 */
            ans = ans * (seq_t)10;          /* shift decimal */
            /* bit 0 contributes nothing */
            prev = 0;
        } else {                            /* choose bit 1 */
            k -= cnt0;
            ans = ans * (seq_t)10 + (seq_t)1;
            prev = 1;
        }
    }

    /* Append the final mandatory 1. */
    ans = ans * (seq_t)10 + (seq_t)1;
    return ans;
}
