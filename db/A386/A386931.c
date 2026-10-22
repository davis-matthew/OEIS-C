/*
 * A386931  a(n) is the smallest positive integer k such that k^2 is equal to
 *          the sum of n consecutive positive integers, or 0 if no such k
 *          exists.
 * https://oeis.org/A386931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt(A076115(n)). a(n) is the smallest k such that k²
 *            equals the sum of n consecutive positive integers, or 0 if no
 *            such k exists
 * Range:     u8 n <= 11, u16 n <= 70, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the congruence
       k² ≡ n(n−1)/2 (mod n)   with k² > n(n−1)/2
   For odd n: the modulus condition reduces to k² ≡ 0 (mod n).  The
   minimal k is the smallest multiple of  √n (ceil of each prime
   exponent/2) that exceeds sqrt(n(n−1)/2).  For even n we brute‑force
   residues modulo n to find all k₀ such that k₀² ≡ n/2 (mod n) and
   choose the smallest member of the arithmetic progression k₀ + t·n
   that surpasses sqrt(n(n−1)/2). */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */

    /* n(n−1)/2 is the threshold for k² */
    seq_t threshold = (seq_t)n * ((seq_t)n - 1) / 2;
    /* smallest integer strictly greater than sqrt(threshold) */
    seq_t bound = floor_sqrt(threshold) + 1;

    if (n & 1) { /* odd n ------------------------------------------------- */
        idx_t temp = n;
        /* s = product of p^{ceil(e/2)} for each prime factor p^e of n */
        seq_t s = 1;
        for (idx_t p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                int exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                int need = (exp + 1) / 2;    /* ceil(exp/2) */
                for (int i = 0; i < need; ++i) {
                    s = mul_small(s, (unsigned int)p);
                }
            }
        }
        if (temp > 1) { /* remaining prime factor */
            s = mul_small(s, (unsigned int)temp);
        }
        /* smallest multiple of s that is ≥ bound */
        seq_t candidate = ((bound + s - 1) / s) * s;
        return candidate;
    } else { /* even n ----------------------------------------------------- */
        seq_t half = (seq_t)n / 2;
        seq_t best = 0;
        for (idx_t k0 = 0; k0 < n; ++k0) {
            seq_t k0sq = (seq_t)k0 * (seq_t)k0;
            if (k0sq % n == half) {            /* k0 satisfies the congruence */
                seq_t candidate;
                if (k0 < bound) {
                    seq_t t = (bound - k0 + n - 1) / n; /* ceil((bound−k0)/n) */
                    candidate = k0 + t * n;
                } else {
                    candidate = k0;                /* already ≥ bound */
                }
                if (best == 0 || candidate < best) {
                    best = candidate;
                }
            }
        }
        return best;
    }
}
