/*
 * A400275  Number of strongly connected components of a directed graph
 *          Gamma_{2,n}(Z) related to wild frieze patterns.
 * https://oeis.org/A400275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, new
 * Formula:   a(n) = C(n+1)/(n+3) + (2/3)*C(n/3) + n/2 (n even) a(n) =
 *            2*C(n+1)/(n+3) + C((n+1)/2) + (4/3)*C(n/3) + (n-1)/2 (n odd)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   catalan, lcm_of_d1_and_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(k) is the k‑th Catalan number (A000108) */
#include "../utils/catalan.h"
#include "../utils/lcm_of_d1_and_3.h"

/* Main function */
seq_t a(idx_t n)
{
    /* d1 = n + 3 */
    idx_t d1 = n + 3;
    /* lcm of d1 and 3 */
    idx_t lcm = lcm_of_d1_and_3(d1);

    /* small integer factors for scaling */
    idx_t factor_d1 = (3 / (d1 % 3 == 0 ? 3 : 1));   /* 1 or 3 */
    idx_t factor_3  = d1 / (d1 % 3 == 0 ? 3 : 1);    /* d1 or d1/3 */

    seq_t numerator = 0;

    if ((n & 1) == 0) { /* even n */
        seq_t termA = catalan(n + 1) * (seq_t)factor_d1;
        seq_t termB = 0;
        if (n % 3 == 0) {
            termB = catalan(n / 3) * 2 * (seq_t)factor_3;
        }
        seq_t termC = (seq_t)(n / 2) * (seq_t)lcm;
        numerator = termA + termB + termC;
    } else {            /* odd n */
        seq_t termA = 2 * catalan(n + 1) * (seq_t)factor_d1;
        seq_t termB = catalan((n + 1) / 2) * (seq_t)lcm;
        seq_t termC = 0;
        if (n % 3 == 0) {
            termC = 4 * catalan(n / 3) * (seq_t)factor_3;
        }
        seq_t termD = ((seq_t)((n - 1) / 2)) * (seq_t)lcm;
        numerator = termA + termB + termC + termD;
    }

    return numerator / (seq_t)lcm;
}
