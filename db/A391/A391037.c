/*
 * A391037  Number of ways to write n as ((p-1)/2)^4 + (q-1)^2/2 + (r-1)^2/4 +
 *          (s+1)^2/4, where each of p,q,r,s is either 1 or an odd prime.
 * https://oeis.org/A391037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 189, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of representations of n according to 
   A391037: ((p-1)/2)^4 + (q-1)^2/2 + (r-1)^2/4 + (s+1)^2/4
   where each of p,q,r,s is 1 or an odd prime. */
#include <stddef.h>          // size_t
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the count of representations for a given n. */
seq_t a(idx_t n)
{
    /* n < 1: zero representations (the OEIS offset is 1, but 0 is legal input) */
    if (n < 1) return 0;

    /* --------------------------------------------------------------------
       Pre‑compute all possible values for each variable that are <= n.
       The arrays are sized conservatively to accommodate the bounds
       for n up to at least 10 000 (the required test range).         */
    /* p values: ((p-1)/2)^4, p = 1 or odd prime */
    seq_t pVals[128];
    size_t pCnt = 0;
    pVals[pCnt++] = 0;               /* p = 1 → value 0 */
    for (idx_t p = 3;; p += 2) {
        if (!is_prime(p)) continue;
        seq_t half = (seq_t)(p - 1) / 2;
        seq_t aVal = half * half * half * half; /* ((p-1)/2)^4 */
        if (aVal > n) break;
        pVals[pCnt++] = aVal;
    }

    /* q values: (q-1)^2 / 2, q = 1 or odd prime */
    seq_t qVals[256];
    size_t qCnt = 0;
    qVals[qCnt++] = 0;               /* q = 1 → value 0 */
    for (idx_t q = 3;; q += 2) {
        if (!is_prime(q)) continue;
        seq_t d = (seq_t)(q - 1);
        seq_t bVal = (d * d) / 2;     /* (q-1)^2 / 2 */
        if (bVal > n) break;
        qVals[qCnt++] = bVal;
    }

    /* r values: (r-1)^2 / 4, r = 1 or odd prime */
    seq_t rVals[512];
    size_t rCnt = 0;
    rVals[rCnt++] = 0;               /* r = 1 → value 0 */
    for (idx_t r = 3;; r += 2) {
        if (!is_prime(r)) continue;
        seq_t d = (seq_t)(r - 1);
        seq_t cVal = (d * d) / 4;     /* (r-1)^2 / 4 */
        if (cVal > n) break;
        rVals[rCnt++] = cVal;
    }

    /* s values: (s+1)^2 / 4, s = 1 or odd prime */
    seq_t sVals[512];
    size_t sCnt = 0;
    sVals[sCnt++] = 1;               /* s = 1 → (1+1)^2/4 = 1 */
    for (idx_t s = 3;; s += 2) {
        if (!is_prime(s)) continue;
        seq_t t = (seq_t)(s + 1);
        seq_t dVal = (t * t) / 4;    /* (s+1)^2 / 4 */
        if (dVal > n) break;
        sVals[sCnt++] = dVal;
    }

    /* --------------------------------------------------------------------
       Count all quadruples (pVal, qVal, rVal, sVal) that sum up to n.  */
    seq_t count = 0;
    for (size_t i = 0; i < pCnt; ++i) {
        seq_t aVal = pVals[i];
        if (aVal > n) continue;
        for (size_t j = 0; j < qCnt; ++j) {
            seq_t ab = aVal + qVals[j];
            if (ab > n) continue;
            for (size_t k = 0; k < rCnt; ++k) {
                seq_t abc = ab + rVals[k];
                if (abc > n) continue;
                seq_t rem = n - abc;
                for (size_t l = 0; l < sCnt; ++l) {
                    if (sVals[l] == rem) {
                        ++count;
                        break;   /* each sVal matches at most once */
                    }
                }
            }
        }
    }

    return count;   /* wrapped modulo the size of seq_t if overflow occurs */
}
