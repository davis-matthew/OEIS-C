/*
 * A377136  Triangle read by rows. Each row is a permutation of a block of
 *          consecutive numbers; the blocks are disjoint and every positive
 *          number belongs to some block. The length of row n is the (n+1)-st
 *          Fibonacci number for n > 0; see Comments.
 * https://oeis.org/A377136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) computes the nth term of A377136. For n≥1 let r be the
 *            unique integer with F(r+3) ≥ n+1 > F(r+2), where F(k) is the
 *            Fibonacci sequence (F(0)=0,F(1)=1). The term lies in row r,
 *            block starting at low = F(r+2) and having length len = F(r+1).
 *            For the position k = n‑low+1 (1‑based) in that row th...
 * Range:     u8 n <= 243, u16 n <= 6764, u32 n <= 6764, u64 n <= 6764,
 *            u128 n <= 6764, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Fast Fibonacci precomputation (sufficient for n up to large values). */
    static seq_t fib[70];
    static bool initialized = false;
    if (!initialized) {
        fib[0] = 0;
        fib[1] = 1;
        for (int i = 2; i < 70; ++i)
            fib[i] = fib[i-1] + fib[i-2];
        initialized = true;
    }

    /* Find r such that F(r+3) >= n+1 > F(r+2). */
    int r = 0;
    while (fib[r+3] < (seq_t)(n + 1))
        ++r;

    /* Limits of the found block. */
    seq_t low = fib[r+2];   /* first value in the row */
    seq_t len = fib[r+1];   /* length of the row = F(r+1)   */

    /* Position inside the row (0‑based). */
    seq_t pos = (seq_t)n - low;          /* pos is 0 ≤ pos < len        */
    seq_t k   = pos + 1;                 /* 1‑based position            */

    /* Compute the permutation offset. */
    seq_t x = 2*k - 1;
    seq_t offset;
    if (x <= len)
        offset = x - 1;                  /* (2*k-2) */
    else
        offset = 2*len + 1 - 2*k;        /* 2*len - 2*k + 1 */

    return low + offset;
}
