/*
 * A242834  Least number k such that k*n/(k+n) is prime or 0 if no such k
 *          exists.
 * https://oeis.org/A242834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that k*n/(k+n) is prime, or 0 if none
 *            exists. The computation uses elementary number‑theoretic facts:
 *            - k*n/(k+n) is integer iff gcd(k,n) is a multiple of k/n
 *            remainder 1. - The prime value equals t*a*b with t,a,b coprime,
 *            thus two of them must be 1. This yields three candidat...
 * Range:     u8 n <= 17, u16 n <= 257, u32 n <= 9833, u64 n <= 9833,
 *            u128 n <= 9833, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n) {
    seq_t best = 0;
    bool found = false;

    /* candidate from n = 2p: k = n */
    if ((n & 1) == 0 && isPrimeTrial((seq_t)(n >> 1))) {
        best = (seq_t)n;
        found = true;
    }

    /* candidate from n = p+1: k = p(p+1) = n*(n-1) */
    if (isPrimeTrial((seq_t)(n - 1))) {
        seq_t cand = (seq_t)n * (seq_t)(n - 1);
        if (!found || cand < best) {
            best = cand;
            found = true;
        }
    }

    /* candidate from n = p(p+1): k = p+1 */
    for (idx_t p = 2; p * p <= n; ++p) {
        if (!isPrimeTrial((seq_t)p))
            continue;
        if (n % p == 0) {
            idx_t m = n / p;
            if (m == p + 1) {
                seq_t cand = (seq_t)(p + 1);
                if (!found || cand < best) {
                    best = cand;
                    found = true;
                }
            }
        }
    }

    return found ? best : (seq_t)0;
}
