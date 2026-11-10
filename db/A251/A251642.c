/*
 * A251642  Records values in A072994.
 * https://oeis.org/A251642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th record (strict left-to-right maximum) of A072994,
 *            where A072994(m) = Prod_{p^e||m} gcd(m, p^(e-1)*(p-1)); computed
 *            by scanning m = 1, 2,
 * Range:     u8 n <= 13, u16 n <= 56, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t A072994_a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 1;
    idx_t m = n;

    for (idx_t d = 2; d <= m / d; ++d) {
        if (m % d == 0) {
            idx_t e = 0;
            while (m % d == 0) {
                m /= d;
                ++e;
            }
            seq_t pw = 1;
            for (idx_t i = 1; i < e; ++i)
                pw *= (seq_t)d;
            seq_t phi = pw * (seq_t)(d - 1);
            seq_t g = gcd((seq_t)n, phi);
            result *= g;
        }
    }

    if (m > 1) {
        seq_t phi = (seq_t)(m - 1);
        seq_t g = gcd((seq_t)n, phi);
        result *= g;
    }

    return result;
}

#define A251642_MAXREC 512

static seq_t A251642_rec[A251642_MAXREC + 1];
static idx_t A251642_count = 0;
static idx_t A251642_next = 1;
static seq_t A251642_best = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > A251642_MAXREC) n = A251642_MAXREC;
    while (A251642_count < n) {
        seq_t v = A072994_a(A251642_next);
        ++A251642_next;
        if (A251642_count == 0 || v > A251642_best) {
            A251642_best = v;
            ++A251642_count;
            A251642_rec[A251642_count] = v;
        }
    }
    return A251642_rec[n];
}
