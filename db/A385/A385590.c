/*
 * A385590  Triangle read by rows, based on Fibonacci numbers: Let i > 1 be
 *          such that F(i) <= n < F(i+1); i.e., i = A130233(n). Then T(n, k) =
 *          F(i-1)^2 + 1 - (i-1) mod 2 + (n - F(i)) * F(i-2) + (k-1) * F(i-1)
 *          where F(k) = A000045(k).
 * https://oeis.org/A385590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r is the row such that r(r-1)/2 < n ≤
 *            r(r+1)/2, k = n - r(r-1)/2, and T(r,k) = F(i-1)^2 + 1 - (i-1)
 *            mod 2 + (r - F(i))*F(i-2) + (k-1)*F(i-1). Here i is the smallest
 *            integer >1 with F(i) ≤ r < F(i+1) and F(m) is Fibonacci with
 *            F(0)=0, F(1)=1
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t N)
{
    /* Compute row r and column k from the global index N. */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (cumulative < N) {
        r++;
        cumulative += r;
    }
    idx_t k = N - (cumulative - r);  /* 1-based column index */

    /* Compute Fibonacci numbers up to the first value > r. */
    /* Maximum r for N <= 10000 is about 140; 200 is ample. */
    static const int MAX_FIB = 200;
    seq_t fib[MAX_FIB];
    fib[0] = 0;
    fib[1] = 1;
    idx_t j = 2;
    while (j < MAX_FIB && fib[j - 1] <= r) {
        fib[j] = fib[j - 1] + fib[j - 2];
        ++j;
    }

    /* Find i such that F(i) ≤ r < F(i+1). */
    idx_t i = 1;
    while (fib[i] <= r) ++i;
    --i;  /* Now fib[i] ≤ r and fib[i+1] > r. */

    seq_t Fi1 = fib[i - 1];  /* F(i-1) */
    seq_t Fi2 = fib[i - 2];  /* F(i-2) */
    seq_t Fip1 = fib[i];     /* F(i)   */

    /* Compute T(r,k). */
    seq_t term = Fi1 * Fi1;                               /* F(i-1)^2 */
    term += (seq_t)(1u - ((i - 1u) & 1u));               /* +1 - (i-1) mod 2 */
    term += ((seq_t)r - Fip1) * Fi2;                     /* + (r-F(i))*F(i-2) */
    term += ((seq_t)k - 1u) * Fi1;                       /* + (k-1)*F(i-1) */

    return term;
}
