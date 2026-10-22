/*
 * A344728  a(n) = (9*n/4 - 51/8 - 5/(16*n-24) + 1/n +
 *          6/(n+1))*binomial(2*n-2,n-1).
 * https://oeis.org/A344728
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n/4 - 51/8 - 5/(16*n-24) + 1/n +
 *            6/(n+1))*binomial(2*n-2,n-1)
 * Range:     u8 none, u16 n <= 4, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* Values below the offset are not required */

    /* Convert index to the sequence type for arithmetic */
    seq_t ns   = (seq_t)n;
    seq_t np1  = ns + 1;                    /* n + 1 */
    seq_t n16m24 = (seq_t)16 * ns - 24;      /* 16*n - 24 */

    /* ------------- Build the numerator N ------------------ */
    /* term1 = 18*n^2*(n+1)*(16*n-24) */
    seq_t term1 = (seq_t)18 * ns * ns * np1 * n16m24;

    /* term2 = 51*n*(n+1)*(16*n-24) */
    seq_t term2 = (seq_t)51 * ns * np1 * n16m24;

    /* term3 = 40*n*(n+1)    (to subtract) */
    seq_t term3 = (seq_t)40 * ns * np1;

    /* term4 = 8*(n+1)*(16*n-24) */
    seq_t term4 = (seq_t)8 * np1 * n16m24;

    /* term5 = 48*n*(16*n-24)     */
    seq_t term5 = (seq_t)48 * ns * n16m24;

    seq_t N = term1 - term2 - term3 + term4 + term5;

    /* ------------- Build the denominator D ---------------- */
    seq_t D = (seq_t)8 * ns * np1 * n16m24;   /* 8*n*(n+1)*(16*n-24) */

    /* ------------- Binomial coefficient ------------------- */
    seq_t B = binomial((idx_t)(2 * n - 2), n - 1);

    /* ------------- Cancel common factors ----------------- */
    seq_t g;

    g = gcd(B, D);
    if (g != 0) { B /= g; D /= g; }

    g = gcd(N, D);
    if (g != 0) { N /= g; D /= g; }

    /* D should now be 1; if not, final division is safe as it is exact. */
    seq_t result = (D == 1) ? B * N : (B * N) / D;
    return result;
}
