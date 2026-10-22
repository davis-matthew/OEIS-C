/*
 * A308641  Number of ways to write n as (2^a*5^b)^2 + c*(3*c+1)/2 +
 *          d*(3*d+1)/2, where a and b are nonnegative integers, and c and d
 *          are integers with c*(3*c+1)/2 <= d*(3*d+1)/2.
 * https://oeis.org/A308641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (a,b,c,d) | (2^a·5^b)^2 + f(c) + f(d) = n, * where a,b
 *            ≥ 0, c,d ∈ ℤ, f(x)=x·(3x+1)/2, and f(c) ≤ f(d) }. * The
 *            algorithm enumerates all square terms (2^a·5^b)^2 ≤ n, then *
 *            counts unordered pairs of f-values summing to the remainder
 * Range:     u8 n <= 608, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* External types defined by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Safe size for the boolean table: n + 1 elements. */
    size_t tbl_size = (size_t)n + 1;
    unsigned char *has = (unsigned char *)calloc(tbl_size, 1);
    if (!has)
        return 0;                /* allocation failed */

    /* Zero‑value f(0) is always present. */
    has[0] = 1;

    /* Generate all values f(k) = k·(3k+1)/2 and f(−k) = k·(3k−1)/2
     * that are ≤ n.  Stop when both sequences exceed n. */
    idx_t k = 1;
    seq_t Nterm = (seq_t)n;      /* N as seq_t for arithmetic */
    for (;; ++k)
    {
        /* Positive side */
        seq_t f_pos = (seq_t)k * ((seq_t)3 * k + 1) / 2;
        if (f_pos <= Nterm)
            has[(size_t)f_pos] = 1;

        /* Negative side (k > 0) */
        seq_t f_neg = (seq_t)k * ((seq_t)3 * k - 1) / 2;
        if (f_neg <= Nterm)
            has[(size_t)f_neg] = 1;

        if (f_pos > Nterm && f_neg > Nterm)
            break;  /* no further values will be ≤ n */

        /* Prevent an infinite loop if n is very large; not expected. */
        if (k > (idx_t)n)
            break;
    }

    /* Count solutions. */
    seq_t total = 0;
    size_t max_root = (size_t)n;          /* scratch holder for pow2*pow2 */

    /* Enumerate all square terms (2^a·5^b)^2 ≤ n. */
    for (size_t a = 0, pow2 = 1; pow2 * pow2 <= n; ++a, pow2 <<= 1)
    {
        for (size_t b = 0, pow5 = 1; ; ++b, pow5 *= 5)
        {
            size_t val = pow2 * pow5;
            size_t sq  = val * val;
            if (sq > n)
                break;                  /* larger b will only increase sq */

            size_t rem = n - sq;
            seq_t cnt = 0;

            /* Count unordered pairs (p,q) with p+q = rem, p ≤ q, p,q ∈ S. */
            for (size_t p = 0; p <= rem / 2; ++p)
                if (has[p] && has[rem - p])
                    cnt++;

            total += cnt;
        }
    }

    free(has);
    return total;
}
