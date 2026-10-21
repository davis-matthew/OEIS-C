/*
 * A244942  Number of length 3 0..n arrays with each partial sum starting from
 *          the beginning no more than sqrt(3) standard deviations from its
 *          mean.
 * https://oeis.org/A244942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (a,b,c) with 0 ≤ a,b,c ≤ n such that
 *            for k = 1,2,3, |S_k – k·μ| ≤ √3·σ, where S_k = a + … + a_k, μ =
 *            (a+b+c)/3, and σ² = [(a–μ)² + (b–μ)² + (c–μ)²]/3. The function
 *            counts all such triples by enumerating admissible ranges for a,
 *            b and c derived from the inequalities
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

/* Integer square root: floor(sqrt(n)) for 0 ≤ n < 2^64 */
static idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    idx_t bit = 1ULL << 62; /* highest position of a bit pair in 64‑bit */
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Bounds for the inequalities derived from the definition */
    idx_t bound1 = n * (n + 2);                 /* for k = 1 */
    idx_t bound2 = 2 * n * (n + 2);              /* for k = 2 */
    idx_t bound3 = 3 * n * (n + 2);              /* for k = 3 */

    idx_t sqrt1 = isqrt_u64(bound1);
    idx_t sqrt2 = isqrt_u64(bound2);
    idx_t sqrt3 = isqrt_u64(bound3);

    /* a  bound (k = 1) */
    int64_t diff_a = (int64_t)n - (int64_t)sqrt1;
    idx_t a_min = (diff_a > 0) ? (idx_t)((diff_a + 1) / 2) : 0;
    idx_t a_max = (n + sqrt1) / 2;

    /* s2 = a + b  bound (k = 2) */
    int64_t diff_s2 = (int64_t)(2 * n) - (int64_t)sqrt2;
    idx_t s2_min = (diff_s2 > 0) ? (idx_t)((diff_s2 + 1) / 2) : 0;
    idx_t s2_max = (2 * n + sqrt2) / 2;

    /* s3 = a + b + c  bound (k = 3) */
    int64_t diff_s3 = (int64_t)(3 * n) - (int64_t)sqrt3;
    idx_t s3_min = (diff_s3 > 0) ? (idx_t)((diff_s3 + 1) / 2) : 0;
    idx_t s3_max = (3 * n + sqrt3) / 2;

    seq_t total = 0;

    if (a_max >= a_min) {
        for (idx_t a = a_min; a <= a_max; ++a) {
            /* Determine allowed range for b from s2 bounds */
            idx_t b_min = (s2_min > a) ? (s2_min - a) : 0;
            idx_t b_max = s2_max - a;
            if (b_max > n) b_max = n;
            if (b_min > b_max) continue;

            for (idx_t b = b_min; b <= b_max; ++b) {
                idx_t s2 = a + b;

                /* Determine allowed range for c from s3 bounds */
                idx_t c_min = (s3_min > s2) ? (s3_min - s2) : 0;
                idx_t c_max = s3_max - s2;
                if (c_max > n) c_max = n;
                if (c_min > c_max) continue;

                /* Add count of admissible c values */
                total += (seq_t)(c_max - c_min + 1);
            }
        }
    }

    return total;
}
