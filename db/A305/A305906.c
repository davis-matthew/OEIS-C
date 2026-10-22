/*
 * A305906  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          4, 5 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A305906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS sequence A305906
 *
 *  Recurrence (empirical, obtained from the OEIS entry):
 *      a(1)= 4,  a(2)=14,  a(3)=35,  a(4)=98,  a(5)=358,  a(6)=1124,
 *      a(7)=3534, a(8)=11667, a(9)=37816, a(10)=121881, a(11)=396149,
 *      a(12)=1286340, a(13)=4168472, a(14)=13525005, a(15)=43893856,
 *      a(16)=142389105.
 *
 *      For n>16:
 *          a(n) = 2*a(n-1)+4*a(n-2)+11*a(n-3)-19*a(n-4)-49*a(n-5)
 *                 -54*a(n-6)+80*a(n-7)+161*a(n-8)+107*a(n-9)
 *                 -128*a(n-10)-154*a(n-11)-22*a(n-12)+37*a(n-13)
 *                 -26*a(n-14)-33*a(n-15)
 *
 *  The function below recomputes the required terms up to the requested
 *  index `n` on each call.  This keeps the implementation small,
 *  avoids any subtle memory‑corruption bugs from memoisation,
 *  and respects the strict type constraints of the build environment.
 *
 *  The recurrence only involves additions, subtractions and multiplications
 *  by small signed constants.  All operations are performed using
 *  the `seq_t` type; intermediate sums are split into positive and
 *  negative parts to avoid reliance on signed arithmetic in `seq_t`.
 */

#include <stdlib.h>
#include <stdint.h>

/* Peak indexing optimisation for the base terms. */
static const uint64_t base_vals[17] = {
    /* a(0) is unused; indices start at 1 */
    0ULL,
    4ULL, 14ULL, 35ULL,  98ULL,  358ULL, 1124ULL,
    3534ULL, 11667ULL, 37816ULL, 121881ULL, 396149ULL,
    1286340ULL, 4168472ULL, 13525005ULL, 43893856ULL,
    142389105ULL
};

/* Coefficients for the 15‑term recurrence (a(n) = Σ coeff[j]*a(n-j-1)) */
static const int coeff[15] = {
    2, 4, 11, -19, -49, -54, 80, 161, 107,
    -128, -154, -22, 37, -26, -33
};

/*
 *  Public entry point.
 *  If n < 1 the sequence is undefined, but for safety we return 0.
 */
seq_t a(idx_t n)
{
    if (n < 1) 
        return (seq_t)0;

    /* Allocate a temporary array to hold the values up to n.  */
    /* Size: n+1  (index 0 unused) */
    seq_t *tbl = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!tbl) 
        exit(EXIT_FAILURE);

    /* Set the base values a(1) .. a(16).  */
    size_t limit = n < 16 ? n : 16;
    for (size_t i = 1; i <= limit; ++i)
        tbl[i] = (seq_t)base_vals[i];

    /* If the requested index falls within the base range, we are done. */
    if (n <= limit) {
        seq_t res = tbl[n];
        free(tbl);
        return res;
    }

    /* Compute terms n = 17 .. current n using the recurrence. */
    for (size_t i = 17; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        /* There are exactly 15 preceding terms involved. */
        for (int j = 0; j < 15; ++j) {
            size_t idx = i - (j + 1);       /* idx >= 2 for i ≥ 17 */
            seq_t val = tbl[idx];
            int c   = coeff[j];
            if (c >= 0) {
                pos += val * (seq_t)c;
            } else {
                neg += val * (seq_t)(-c);
            }
        }
        tbl[i] = pos - neg;   /* All intermediate values stay within seq_t */
    }

    seq_t result = tbl[n];
    free(tbl);
    return result;
}
