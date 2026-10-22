/*
 * A379954  Smallest positive k such that (n*k)^3/(n^3+k^3) is an integer, or
 *          0 if no such k exists.
 * https://oeis.org/A379954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is smallest k > 0 such that (n*k)^3/(n^3 + k^3) is an
 *            integer; 0 if no such k exists. Computed by brute‑force over all
 *            divisors of n
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 3408, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Return the smallest k that satisfies the condition, or 0 if none. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined, safe return */

    seq_t best_k = 0;        /* 0 means not found yet */

    /* Iterate over all positive divisors d of n. */
    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* Process divisor g = d. */
        seq_t d_seq = (seq_t)d;
        seq_t a_val = (seq_t)(n / d);          /* a = n / g */
        seq_t G3     = d_seq * d_seq * d_seq;  /* d^3 */

        for (uint64_t b = 1; b <= d; ++b) {
            seq_t b_seq = (seq_t)b;
            if (gcd(a_val, b_seq) != 1)
                continue;
            seq_t T = a_val * a_val * a_val + b_seq * b_seq * b_seq;   /* a^3 + b^3 */
            if (T == 0)
                continue;
            if (G3 % T == 0) {
                seq_t k_val = d_seq * b_seq;
                if (best_k == 0 || k_val < best_k)
                    best_k = k_val;
            }
        }

        /* Process complementary divisor g2 = n/d (if distinct). */
        uint64_t d2 = n / d;
        if (d2 == d)
            continue;

        seq_t d2_seq = (seq_t)d2;
        seq_t a_val2 = (seq_t)(n / d2);          /* a = n / g2 = d */
        seq_t G32    = d2_seq * d2_seq * d2_seq;  /* d2^3 */

        for (uint64_t b = 1; b <= d2; ++b) {
            seq_t b_seq = (seq_t)b;
            if (gcd(a_val2, b_seq) != 1)
                continue;
            seq_t T = a_val2 * a_val2 * a_val2 + b_seq * b_seq * b_seq;
            if (T == 0)
                continue;
            if (G32 % T == 0) {
                seq_t k_val = d2_seq * b_seq;
                if (best_k == 0 || k_val < best_k)
                    best_k = k_val;
            }
        }
    }

    return best_k;      /* 0 if no suitable k was found */
}
