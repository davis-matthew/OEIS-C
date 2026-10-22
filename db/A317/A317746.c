/*
 * A317746  Irregular triangle read by rows in which row n lists the divisors
 *          k of n such that k^n + n^k == 0 (mod k + n).
 * https://oeis.org/A317746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of OEIS sequence A317746.
   The sequence is obtained by flattening the triangle of divisors
   k of n such that `k^n + n^k ≡ 0 (mod k+n)`.  The terms are
   read row‑by‑row in increasing order of n and within each row
   in ascending order of k.  The function computes the first
   k that appears at the required position. */

#include "../utils/mod_pow.h"
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t target = n;
    idx_t count  = 0;

    for (idx_t m = 1; ; ++m) {
        /* array to hold the larger divisors in reverse order */
        idx_t big[2048];
        size_t bigcnt = 0;

        /* process the small divisors (including sqrt(m) once) */
        for (idx_t d = 1; d <= m / d; ++d) {
            if (m % d == 0) {
                idx_t k = d;
                seq_t modv = (seq_t)(k + m);
                seq_t sum = (mod_pow((seq_t)k, m, modv) +
                             mod_pow((seq_t)m, k, modv)) % modv;
                if (sum == 0) {
                    ++count;
                    if (count == target) return (seq_t)k;
                }
                idx_t other = m / d;
                if (other != d) big[bigcnt++] = other;
            }
        }

        /* process the larger divisors in ascending order */
        for (size_t i = 0; i < bigcnt; ++i) {
            idx_t k = big[bigcnt - 1 - i];  /* reverse order */
            seq_t modv = (seq_t)(k + m);
            seq_t sum = (mod_pow((seq_t)k, m, modv) +
                         mod_pow((seq_t)m, k, modv)) % modv;
            if (sum == 0) {
                ++count;
                if (count == target) return (seq_t)k;
            }
        }
    }
}
