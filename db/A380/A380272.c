/*
 * A380272  a(n) is the number of integers k in 0..n such that the nonadjacent
 *          forms for n-k and k can be added without carries (see Comments
 *          section for exact definition).
 * https://oeis.org/A380272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of k in [0..n] such that the non‑adjacent *
 *            representations of n-k and k can be added without carries
 * Range:     u8 n <= 511, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 95 terms through n = 8151 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Compute the NAF (Non‑Adjacent Form) of m.
 * The digits are stored from least‑significant (index 0) to most‑significant.
 *  digits[i] will contain -1, 0 or +1.
 *  len will hold how many digits have been written.  Any positions beyond
 *  len are considered zero.  The caller must zero out the array before
 *  calling, or ignore unused positions. */
static void compute_naf(idx_t m, int8_t *digits, int *len)
{
    int i = 0;
    while (m > 0) {
        if (m & 1U) {
            /* odd case: choose +1 or -1 to make the remainder even */
            int mod4 = m & 3U;          /* m % 4 */
            int di = (mod4 == 1) ? 1 : -1;
            digits[i] = (int8_t)di;
            m -= (idx_t)di;
        } else {
            digits[i] = 0;
        }
        m >>= 1;
        ++i;
    }
    *len = i;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    /* Allocate fixed arrays large enough for all NAFs up to the limits in the test set. */
    int8_t nafa[64];
    int8_t nafb[64];
    int lena, lenb;

    for (idx_t k = 0; k <= n; ++k) {
        /* NAF of k */
        memset(nafa, 0, sizeof(nafa));
        compute_naf(k, nafa, &lena);

        /* NAF of n-k */
        idx_t mk = n - k;
        memset(nafb, 0, sizeof(nafb));
        compute_naf(mk, nafb, &lenb);

        int maxlen = (lena > lenb) ? lena : lenb;
        bool ok = true;
        for (int i = 0; i < maxlen; ++i) {
            int s = nafa[i] + nafb[i];
            if (s > 1 || s < -1) {
                ok = false;
                break;
            }
            if (s != 0 && i + 1 < maxlen) {
                int snext = nafa[i + 1] + nafb[i + 1];
                if (snext != 0) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok)
            ++count;
    }

    return count;
}
