/*
 * A338954  Number of chiral pairs of colorings of the 96 edges (or triangular
 *          faces) of the 4-D 24-cell using subsets of a set of n colors.
 * https://oeis.org/A338954
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (96*n^8 + 144*n^12 - 48*n^16 - 64*n^18 - 192*n^20 -
 *            60*n^24 + 48*n^32 + 32*n^36 - 5*n^48 + 72*n^50 - 12*n^52 -
 *            12*n^60 + n^96) / 1152
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 2, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* int64_t */

seq_t a(idx_t n)
{
    /* the sequence is defined from n = 2 upwards; below that offset, return
       0 – callers may ignore these values. */
    if (n < 2)
        return (seq_t)0;

    /* terms of the numerator polynomial: exponent and coefficient. */
    static const struct {
        unsigned exp;
        int64_t  coeff;
    } terms[] = {
        {  8,  96},
        { 12, 144},
        { 16, -48},
        { 18, -64},
        { 20, -192},
        { 24, -60},
        { 32,  48},
        { 36,  32},
        { 48,  -5},
        { 50,  72},
        { 52, -12},
        { 60, -12},
        { 96,   1}
    };
    const size_t num_terms = sizeof terms / sizeof terms[0];

    seq_t n_seq = (seq_t)n;
    seq_t pow   = (seq_t)1; /* will hold n^k */
    size_t term_idx = 0;

    seq_t pos = (seq_t)0; /* sum of positive Product terms */
    seq_t neg = (seq_t)0; /* sum of absolute value of negative Product terms */

    /* compute successive powers n^k and accumulate terms that match */
    for (unsigned k = 1; k <= 96; ++k) {
        pow *= n_seq; /* pow = n^k */

        /* add all terms whose exponent equals current k */
        while (term_idx < num_terms && terms[term_idx].exp == k) {
            int64_t coeff = terms[term_idx].coeff;
            seq_t prod = pow * (seq_t)(coeff > 0 ? coeff : -coeff);
            if (coeff > 0)
                pos += prod;
            else
                neg += prod;
            ++term_idx;
        }
    }

    seq_t numerator = pos - neg;       /* exact dividend */
    seq_t result    = numerator / (seq_t)1152;  /* exact division */
    return result;
}
