/*
 * A303540  Number of ways to write n as a^2 + b^2 + binomial(2*c,c) +
 *          binomial(2*d,d), where a,b,c,d are nonnegative integers with a <=
 *          b and c <= d.
 * https://oeis.org/A303540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 5953, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A303540: number of representations of n as
   a^2 + b^2 + binomial(2c,c) + binomial(2d,d),
   with a,b,c,d non‑negative, a <= b, c <= d */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"      /* binomial(2*k,k)   */
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    /* List all central binomial coefficients <= n */
    seq_t central[256];
    idx_t central_len = 0;

    for (idx_t k = 0; k < 256; ++k) {
        seq_t B = binomial(2 * k, k);
        if (B > n_seq) break;
        central[central_len++] = B;
    }

    seq_t count = 0;

    for (idx_t ic = 0; ic < central_len; ++ic) {
        seq_t Bc = central[ic];
        for (idx_t id = ic; id < central_len; ++id) {   /* c <= d */
            seq_t Bd = central[id];
            seq_t sum_cd = Bc + Bd;
            if (sum_cd > n_seq) continue;

            seq_t m = n_seq - sum_cd;  /* remaining for a^2+b^2 */

            for (idx_t a_val = 0; (seq_t)a_val * a_val <= m; ++a_val) {
                seq_t a_sq = (seq_t)a_val * a_val;
                seq_t b_sq = m - a_sq;
                seq_t b_val = floor_sqrt(b_sq);
                if (b_val * b_val == b_sq && b_val >= a_val) {
                    ++count;
                }
            }
        }
    }
    return count;
}
