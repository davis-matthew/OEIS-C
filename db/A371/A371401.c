/*
 * A371401  Triangle read by rows: T(n, k) = [x^k] (n*x +
 *          1)*Hypergeometric([-n, -n + 1], [1], x).
 * https://oeis.org/A371401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = C(n,k)*C(n-1,k) + n*C(n,k-1)*C(n-1,k-1) The sequence
 *            a(n) is the flattened triangle of T(n,k) read by rows
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, isqrt_8n_plus1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"
#include "../utils/isqrt_8n_plus1.h"

/* find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static inline idx_t find_row(idx_t n)
{
    /* initial high bound from double sqrt, a safe overestimate */
    idx_t high = (idx_t)(isqrt_8n_plus1(n) / 2.0L + 2.0L);
    idx_t low = 0;
    idx_t r = 0;
    while (low <= high)
    {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
        {
            r = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break; /* prevent infinite loop on underflow */
            high = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k */
    idx_t r = find_row(n);
    __int128 start128 = (__int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t k = n - start;

    /* r == 0 handles the first row */
    if (r == 0)
        return 1;

    seq_t C1 = binomial(r, k);
    seq_t C2 = binomial(r - 1, k);
    seq_t res = C1 * C2;               /* S_k */

    if (k > 0)
    {
        seq_t C3 = binomial(r, k - 1);
        seq_t C4 = binomial(r - 1, k - 1);
        seq_t Sprev = C3 * C4;           /* S_{k-1} */
        res = res + (seq_t)r * Sprev;
    }

    return res;
}
