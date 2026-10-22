/*
 * A352361  Array read by ascending antidiagonals. A(n, k) = Fibonacci(k, n),
 *          where Fibonacci(n, x) are the Fibonacci polynomials.
 * https://oeis.org/A352361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, changed
 * Formula:   a(n) = read(A(n,k) = Fibonacci polynomial F_k(n)) along *
 *            ascending antidiagonals. For each non‑negative integer s, list *
 *            the pairs (n,k) with n+k = s in the order n = s, s-1, … ,0, *
 *            and output A(n,k) = F_k(n). The sequence a(n) is the
 *            concatenation * of all these values. Algorithm: find the di...
 * Range:     u8 n <= 49, u16 n <= 98, u32 n <= 221, u64 n <= 514,
 *            u128 n <= 1257, big any n
 * Helpers:   fib_poly
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_poly.h"

seq_t a(idx_t n)
{
    /* Find the smallest s with (s+1)(s+2)/2 > n. */
    idx_t s = 0;
    for (;; ++s)
    {
        seq_t cum = ((seq_t)(s + 1) * (s + 2)) / 2;
        if (cum > (seq_t)n) break;
    }

    /* Previous cumulative count: T(s-1) = s(s+1)/2. */
    seq_t prev_cum = ((seq_t)s * (s + 1)) / 2;
    idx_t offset   = (idx_t)((seq_t)n - prev_cum);  /* 0 .. s */
    idx_t k        = offset;                        /* index in Fibonacci polynomial */
    idx_t n_val    = s - offset;                    /* n for the polynomial */

    return fib_poly((seq_t)n_val, k);
}
