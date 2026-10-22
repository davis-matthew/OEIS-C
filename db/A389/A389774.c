/*
 * A389774  Number of solid partitions of n with 7 parts.
 * https://oeis.org/A389774
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ coeff_k · p7(n−k) where p7(m) is the number of
 *            partitions of m using parts 1 … 7. The coefficients come from
 *            the numerator of the generating function (3*q^24 + 20*q^23 +
 *            3*q^22 + 64*q^21 + 109*q^20 + 171*q^19 + 168*q^18 + 268*q^17 +
 *            323*q^16 + 460*q^15 + 326*q^14 + 337*q^13 + 270*q^12 +...
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const struct {
    idx_t shift;          /* exponent of q in the numerator   */
    seq_t coeff;          /* corresponding coefficient        */
} NUMER_COEFFS[] = {
    { 7,  (seq_t)86  },
    { 9,  (seq_t)127 },
    {10,  (seq_t)208 },
    {11,  (seq_t)264 },
    {12,  (seq_t)270 },
    {13,  (seq_t)337 },
    {14,  (seq_t)326 },
    {15,  (seq_t)460 },
    {16,  (seq_t)323 },
    {17,  (seq_t)268 },
    {18,  (seq_t)168 },
    {19,  (seq_t)171 },
    {20,  (seq_t)109 },
    {21,  (seq_t)64  },
    {22,  (seq_t)3   },
    {23,  (seq_t)20  },
    {24,  (seq_t)3   }
};
#define NUM_COEFFS (sizeof(NUMER_COEFFS)/sizeof(NUMER_COEFFS[0]))

/* Cache for the partition numbers p7(m) (parts 1..7) */
static seq_t *P = NULL;
static size_t P_cap = 0;

/* Compute p7(0..N) freshly */
static void compute_p(idx_t N)
{
    if (N + 1 > P_cap) {
        seq_t *tmp = (seq_t *)realloc(P, (N + 1) * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        P = tmp;
        P_cap = N + 1;
    }
    for (idx_t i = 0; i <= N; ++i) P[i] = 0;   /* initialise */
    P[0] = 1;
    for (idx_t coin = 1; coin <= 7; ++coin) {
        for (idx_t sum = coin; sum <= N; ++sum) {
            P[sum] += P[sum - coin];
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 7) return 0;                 /* offset is n = 7 */

    idx_t max_needed = n - 7;             /* largest index we need in P */
    compute_p(max_needed);

    seq_t total = 0;
    for (size_t i = 0; i < NUM_COEFFS; ++i) {
        idx_t shift = NUMER_COEFFS[i].shift;
        if (n >= shift) {
            idx_t idx = n - shift;
            total += NUMER_COEFFS[i].coeff * P[idx];
        }
    }
    return total;
}
