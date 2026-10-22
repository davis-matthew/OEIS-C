/*
 * A342515  Number of strict partitions of n with constant (equal)
 *          first-quotients.
 * https://oeis.org/A342515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A342515: Number of strict partitions of n with constant first-quotients.
 * 
 * A partition λ = (λ1 > λ2 > ... > λk) has constant first-quotients iff
 *   λ_{i+1}/λ_i = r   for all i,
 * where r is a fixed rational < 1.  Then λ_i = s * q^{k-i} * p^{i-1}
 *        (p/q in lowest terms, 1 ≤ p < q, gcd(p,q)=1, k ≥ 1, s ≥ 1).
 * The sum of the parts equals 
 *        n = s * (q^k - p^k)/(q - p).
 * 
 * Conversely, each quadruple (k,p,q,s) with the above properties produces
 * a unique strict partition of n with constant quotients.
 * 
 * Thus for n > 0:
 *   a(n) = 1  (for the one‑part partition) +
 *          Σ_{k=2}^{kmax} #{ (p,q) : 1 ≤ p < q, gcd(p,q)=1,
 *                                 D = (q^k - p^k)/(q - p) ≤ n,
 *                                 D divides n } ,
 *   where kmax = max{ k : 2^k - 1 ≤ n } .
 * 
 * For n = 0 we return 1 (the empty partition).
 *
 * The implementation enumerates all admissible (k,p,q) pairs and checks
 * whether the corresponding D divides n.  For k = 2 the divisor
 * D = p + q is used directly, which allows a simple bound on q.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"

/* Compute gcd for 64‑bit unsigned integers */

/* Compute x^e for unsigned 64‑bit integers, e ≥ 0. */
static inline uint64_t pow_uint64(uint64_t x, int e)
{
    uint64_t r = 1;
    while (e--) r *= x;
    return r;
}

seq_t a(idx_t n)
{
    /* Handle the offset n=0 */
    if (n == 0) return (seq_t)1;

    /* The one‑part partition is always counted */
    unsigned long long ans = 1ULL;
    unsigned long long NN = (unsigned long long)n;

    /* Determine maximal k such that 2^k - 1 <= n */
    int kmax = 1;
    while (((uint64_t)1 << kmax) - 1 <= NN) kmax++;
    if (kmax < 2) return (seq_t)ans;        /* only the single‑part partition */

    for (int k = 2; k <= kmax; ++k) {
        /* Maximal p such that a minimal D could be ≤ n.
           For k = 2, simply use 2p + 1 <= n → p <= (n - 1)/2. */
        unsigned long long pmax = (k == 2) ? (NN - 1) / 2 : (NN - 1) / 2;
        for (unsigned long long p = 1; p <= pmax; ++p) {
            /* Pre‑compute p^k for this k, to reuse in the inner loop */
            unsigned long long pk = 1;
            for (int i = 0; i < k; ++i) pk *= p;

            if (k == 2) {
                /* D = p + q, with q = p+1 .. n-p */
                unsigned long long qmin = p + 1;
                unsigned long long qmax = NN - p;
                for (unsigned long long q = qmin; q <= qmax; ++q) {
                    if (gcd_uint64(p, q) != 1) continue;
                    unsigned long long D = p + q;
                    if (NN % D == 0) ++ans;
                }
            } else {
                /* Outer loop over q; break when D > n */
                unsigned long long q = p + 1;
                while (true) {
                    if (gcd_uint64(p, q) != 1) {
                        ++q;
                        continue;
                    }
                    unsigned long long qk = pow_uint64(q, k);
                    unsigned long long diff = qk - pk;          /* q^k - p^k > 0 */
                    unsigned long long denom = q - p;            /* q > p >= 1 */
                    /* Compute D = (q^k - p^k) / (q - p) */
                    unsigned long long D = diff / denom;        /* division exact */
                    if (D > NN) break;                          /* exceeded bound */
                    if (NN % D == 0) ++ans;
                    ++q;
                }
            }
        }
    }

    return (seq_t)ans;
}
