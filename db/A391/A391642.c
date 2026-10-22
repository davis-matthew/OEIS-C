/*
 * A391642  Number of integer compositions whose first sums are the n-th
 *          composition in standard order.
 * https://oeis.org/A391642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of positive integer solutions (x_0,…,x_k) *
 *            satisfying x_i + x_{i+1} = c[i] for the n‑th composition c in
 *            standard * order (graded reverse‑lexicographic)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th composition in standard order and counts the
 * number of compositions whose first sums equal that composition. */
#include <limits.h>

seq_t a(idx_t n)
{
    /* --- Determine the sum 's' of the n‑th composition ----------------- */
    /* The first 2^s - 1 terms correspond to all compositions of sum s.  */
    /* So s = floor(log2(n)) + 1.                                        */
    idx_t msb = 63 - __builtin_clzll(n);          /* highest set bit of n */
    idx_t s    = msb + 1;                         /* sum of the composition */

    /* Base index: number of terms before sum s. */
    idx_t base = ((idx_t)1 << (s-1)) - 1;
    /* Zero‑based offset inside sum s. */
    idx_t r0   = n - base - 1;                    /* 0 <= r0 < 2^(s-1) */

    /* --- Build the target composition --------------------------------- */
    /* The 'cuts' are obtained from the reversed binary representation
     * of r0.  There are s-1 cut positions. */
    seq_t comp[65];       /* maximum s is 64 */
    idx_t k = 0;          /* number of parts of the target composition */

    idx_t part_len = 1;
    for (idx_t pos = 0; pos < s-1; ++pos)
    {
        /* Extract the bit at the mirrored position. */
        idx_t bit = (r0 >> (s-2-pos)) & 1ULL;

        if (bit)
        {
            comp[k++] = part_len;
            part_len = 1;
        }
        else
        {
            ++part_len;
        }
    }
    /* Push the last part. */
    comp[k++] = part_len;   /* k = length of the composition */

    /* --- Compute bounds on the first part x_0 ------------------------ */
    /* x_0 + x_1 = c[0], x_1 + x_2 = c[1], … .   Solve recursively. */
    long long L = 1;            /* lower bound on x_0 (must be >=1) */
    long long U = LLONG_MAX;    /* upper bound, LLONG_MAX indicates none */
    int      alpha = 1;         /* coefficient of x_0 in the current x_i */
    long long beta  = 0;        /* constant term of the current x_i */

    for (idx_t idx = 0; idx < k; ++idx)
    {
        /* Impose x_i >= 1 */
        if (alpha == 1)
        {
            long long lower = 1 - beta;
            if (lower > L) L = lower;
        }
        else
        {
            long long upper = beta - 1;
            if (upper < U) U = upper;
        }

        /* Compute next x_{i+1} = c[i] - x_i */
        long long next_beta = (long long)comp[idx] - beta;
        alpha = -alpha;
        beta  = next_beta;
    }

    /* Impose inequality for x_k */
    if (alpha == 1)
    {
        long long lower = 1 - beta;
        if (lower > L) L = lower;
    }
    else
    {
        long long upper = beta - 1;
        if (upper < U) U = upper;
    }

    /* --- Count solutions ------------------------------------------------- */
    long long count = 0;
    if (L <= U)
        count = U - L + 1;
    if (count < 0) count = 0;

    return (seq_t)count;
}
