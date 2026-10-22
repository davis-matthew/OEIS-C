/*
 * A338965  Number of unoriented colorings of the 120 dodecahedral facets of
 *          the 4-D 120-cell (or 120 vertices of the 4-D 600-cell) using
 *          subsets of a set of n colors.
 * https://oeis.org/A338965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (960*n^4 + 1440*n^6 + 960*n^8 + 1200*n^10 + 336*n^12 +
 *            288*n^16 + 1440*n^17 + 1440*n^19 + 40*n^20 + 400*n^22 +
 *            1200*n^23 + 336*n^24 + 1200*n^27 + 60*n^30 + 1800*n^31 +
 *            288*n^32 + 40*n^40 + 400*n^44 + 1*n^60 + 60*n^61 + 450*n^62 +
 *            60*n^75 + 1*n^120) / 14400
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

typedef struct {
    int k;              /* exponent of n */
    uint64_t c;         /* coefficient for n^k */
} coeff_t;

static const coeff_t coeffs[] = {
    {  4, 960ULL},
    {  6, 1440ULL},
    {  8, 960ULL},
    { 10, 1200ULL},
    { 12, 336ULL},
    { 16, 288ULL},
    { 17, 1440ULL},
    { 19, 1440ULL},
    { 20,  40ULL},
    { 22, 400ULL},
    { 23, 1200ULL},
    { 24, 336ULL},
    { 27, 1200ULL},
    { 30,  60ULL},
    { 31, 1800ULL},
    { 32, 288ULL},
    { 40,  40ULL},
    { 44, 400ULL},
    { 60,   1ULL},
    { 61,  60ULL},
    { 62, 450ULL},
    { 75,  60ULL},
    {120,   1ULL}
};

static const size_t coeff_count = sizeof(coeffs) / sizeof(coeffs[0]);

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t sn = (seq_t)n;
    seq_t sum = (seq_t)0;

    for (size_t i = 0; i < coeff_count; ++i) {
        /* compute sn^k */
        seq_t pow_val = (seq_t)1;
        for (idx_t j = 0; j < coeffs[i].k; ++j)
            pow_val *= sn;

        sum += (seq_t)coeffs[i].c * pow_val;
    }

    /* divide by group order 14400 */
    return sum / (seq_t)14400;
}
