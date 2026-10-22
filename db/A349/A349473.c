/*
 * A349473  Irregular triangle read by rows: the n-th row contains the
 *          elements in the continued fraction of the harmonic mean of the
 *          divisors of n.
 * https://oeis.org/A349473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th element of the flattened irregular triangle.
 *            Each row m contains the continued‑fraction coefficients of the
 *            harmonic mean of the divisors of m. The function generates rows
 *            sequentially, stores all coefficients in a dynamically resized
 *            array, and returns the coefficient at index n...
 * Range:     u8 n <= 55, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   gcd, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Cache for all coefficients generated so far */
    static seq_t *coeffs = NULL;
    static size_t coeffs_len = 0;   /* number of coeffs stored   */
    static size_t coeffs_cap = 0;   /* capacity of coeffs array   */
    static idx_t last_m = 0;        /* highest m processed so far */

    /* Generate rows until we have at least n coefficients */
    while (coeffs_len < (size_t)n)
    {
        /* Next integer m to process */
        idx_t m = last_m + 1;
        last_m = m;

        /* ---- Compute the harmonic mean of divisors of m ---- */
        seq_t sum_num = 0;       /* numerator of the sum of reciprocals   */
        seq_t sum_den = 1;       /* denominator of the same sum          */
        idx_t div_count = 0;     /* number of divisors (k)                */

        /* Enumerate divisors efficiently using pairs (d, m/d) */
        for (idx_t d = 1; d <= m / d; ++d)
        {
            if (m % d == 0)
            {
                /* divisor d */
                ++div_count;
                seq_t new_num = sum_num * (seq_t)d + sum_den;
                seq_t new_den = sum_den * (seq_t)d;
                seq_t g = gcd(new_num, new_den);
                sum_num = new_num / g;
                sum_den = new_den / g;

                /* paired divisor m/d, if distinct */
                idx_t d2 = m / d;
                if (d2 != d)
                {
                    ++div_count;
                    new_num = sum_num * (seq_t)d2 + sum_den;
                    new_den = sum_den * (seq_t)d2;
                    g = gcd(new_num, new_den);
                    sum_num = new_num / g;
                    sum_den = new_den / g;
                }
            }
        }

        /* Harmonic mean H = k / (sum_num/sum_den) = k*sum_den / sum_num */
        seq_t H_num = (seq_t)div_count * sum_den;
        seq_t H_den = sum_num;
        seq_t g = gcd(H_num, H_den);
        H_num /= g;
        H_den /= g;

        /* ---- Continued‑fraction expansion of H = H_num/H_den ---- */
        while (H_den != (seq_t)0)
        {
            seq_t a_coeff = H_num / H_den;   /* integer part */

            /* Grow the coefficient array if necessary */
            if (coeffs_len == coeffs_cap)
            {
                size_t new_cap = coeffs_cap ? coeffs_cap * 2 : 32;
                seq_t *new_arr = seq_alloc_new_array(new_cap);
                /* Copy existing coefficients */
                for (size_t i = 0; i < coeffs_len; ++i)
                    new_arr[i] = coeffs[i];
                /* Free old array */
                if (coeffs != NULL)
                    seq_free_array(coeffs);
                coeffs = new_arr;
                coeffs_cap = new_cap;
            }

            coeffs[coeffs_len++] = a_coeff;

            /* Next remainder in Euclid's algorithm */
            seq_t rem = H_num % H_den;
            H_num = H_den;
            H_den = rem;
        }
    }

    /* Return the requested coefficient */
    return coeffs[(size_t)(n) - 1];
}
