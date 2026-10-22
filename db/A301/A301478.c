/*
 * A301478  a(n) is the product of the n-th term of A301382 and its initial
 *          digit.
 * https://oeis.org/A301478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A301478
 *  a(n) is the product of the n-th term of A301382 and its first digit.
 *
 *  The sequence is obtained by merging the n-digit blocks {d*10^m,…,(d+1)*10^m-1}
 *  (with leading digit d, 1 ≤ d ≤ 9, m ≥ 0) according to the value d*x.
 *  If a duplicate product occurs it is discarded (the sequence is strictly
 *  increasing).
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* offset is 1; n = 0 is unused */
        return (seq_t)0;

    /* A stream handles all numbers whose leading digit is d. */
    struct Stream {
        seq_t x;        /* current number */
        seq_t pow10;    /* 10^m for the current digit length */
        seq_t upper;    /* maximal number in the current block */
        unsigned d;     /* fixed leading digit */
    } stream[10];            /* indices 1 … 9 used */

    /* initialise each stream to the first number with that leading digit */
    for (unsigned d = 1; d <= 9; ++d) {
        stream[d].d     = d;
        stream[d].pow10 = (seq_t)1;
        stream[d].x     = (seq_t)d;
        stream[d].upper = (seq_t)(d + 1) - (seq_t)1; /* (d+1)*1 - 1 = d */
    }

    seq_t prev_result = (seq_t)0;   /* previous (unique) product produced */
    seq_t result = (seq_t)0;
    idx_t  count  = 0;              /* number of unique products produced */

    while (count < n) {
        /* Find the stream with minimal current product */
        unsigned min_idx = 1;
        seq_t min_prod = stream[1].x * (seq_t)stream[1].d;

        for (unsigned d = 2; d <= 9; ++d) {
            seq_t prod = stream[d].x * (seq_t)stream[d].d;
            if (prod < min_prod) {
                min_prod = prod;
                min_idx  = d;
            }
        }

        /* If this product repeats the previous one, skip it */
        if (min_prod == prev_result) {
            /* advance the chosen stream and restart the loop */
            stream[min_idx].x += (seq_t)1;
            if (stream[min_idx].x > stream[min_idx].upper) {
                stream[min_idx].pow10 *= (seq_t)10;
                stream[min_idx].x = stream[min_idx].pow10 * (seq_t)stream[min_idx].d;
                stream[min_idx].upper = stream[min_idx].pow10 * (seq_t)(stream[min_idx].d + 1) - (seq_t)1;
            }
            continue;
        }

        /* We have found the next unique product */
        result      = min_prod;
        prev_result = result;
        ++count;

        /* Advance the stream that produced this product */
        stream[min_idx].x += (seq_t)1;
        if (stream[min_idx].x > stream[min_idx].upper) {
            stream[min_idx].pow10 *= (seq_t)10;
            stream[min_idx].x = stream[min_idx].pow10 * (seq_t)stream[min_idx].d;
            stream[min_idx].upper = stream[min_idx].pow10 * (seq_t)(stream[min_idx].d + 1) - (seq_t)1;
        }
    }

    return result;
}
