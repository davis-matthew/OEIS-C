/*
 * A286239  Triangular table: T(n,k) = 0 if k does not divide n, otherwise
 *          T(n,k) = P(A000010(n/k), k), where P is sequence A000027 used as a
 *          pairing function N x N -> N. Table is read by rows as T(1,1),
 *          T(2,1), T(2,2), etc.
 * https://oeis.org/A286239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 299, u16 n <= 9433, u32 n <= 9433, u64 n <= 9433,
 *            u128 n <= 9433, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(r, k) where
 *   r = smallest integer with r*(r+1)/2 ≥ n
 *   k = n − r*(r−1)/2
 *   T(r, k) = 0                     if k ∤ r
 *   T(r, k) = P(φ(r/k), k)          otherwise
 *   P(x, y) = (1/2)*(2 + (x + y)² − x − 3*y)
 *   φ(m) is Euler's totient function.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* Euler's totient for a uint64_t argument.  Returns a seq_t. */
static seq_t
phi_uint64(idx_t m)
{
    if (m <= 1) return (seq_t)m;   /* φ(0)=0, φ(1)=1 */
    seq_t result = (seq_t)m;
    idx_t temp = m;
    for (idx_t p = 2; (uint64_t)p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result = result / (seq_t)p * (seq_t)(p - 1);
        }
    }
    if (temp > 1)
        result = result / (seq_t)temp * (seq_t)(temp - 1);
    return result;
}

/* Cantor pairing function P(x, y) expressed in integer operations. */
static seq_t
cantor_pair(seq_t x, seq_t y)
{
    seq_t sum = x + y;
    seq_t sq  = sum * sum;
    seq_t num = (seq_t)2 + sq - x - (seq_t)3 * y;
    return num / (seq_t)2;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* n is defined for ≥1, but guard */

    /* Find row r such that r*(r+1)/2 ≥ n.  Use long double sqrt for
       a rough starting point, then correct by integer comparisons. */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    long double d = sqrt((long double)val);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct rounding errors. */
    while ( (unsigned __int128)r * (r + 1) / 2 < n ) ++r;
    while ( r > 0 && (unsigned __int128)(r - 1) * r / 2 >= n ) --r;

    /* Column index k inside the row. */
    idx_t base = (idx_t)((unsigned __int128)r * (r - 1) / 2);
    idx_t k = n - base;

    /* Compute T(r, k). */
    if (k == 0) return (seq_t)0;   /* guard, though k starts from 1 */

    if (r % k != 0)
        return (seq_t)0;

    idx_t m = r / k;              /* m = r/k */
    seq_t phi_m = phi_uint64(m);
    return cantor_pair(phi_m, (seq_t)k);
}
