/*
 * A344597  a(n) = Sum_{k=1..n} mu(k) * (floor(n/k)^4 - floor((n-1)/k)^4).
 * https://oeis.org/A344597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} μ(k) * (⌊n/k⌋⁴ - ⌊(n-1)/k⌋⁴)
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int mobius(idx_t x) {
    if (x == 1) return 1;
    idx_t n = x;
    int primes = 0;
    int mu = 1;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int cnt = 0;
            do {
                n /= p;
                cnt++;
            } while (n % p == 0);
            if (cnt > 1) return 0;       /* square factor */
            primes++;                     /* distinct prime */
        }
    }
    if (n > 1) primes++;                  /* remaining prime factor */
    return (primes & 1) ? -1 : 1;         /* parity of prime count */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    const idx_t n_minus_1 = n - 1;
    int64_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        int mu = mobius(k);
        if (mu == 0) continue;

        const idx_t qn   = n   / k;
        const idx_t qn1  = n_minus_1 / k;
        const seq_t qn_seq  = (seq_t)qn;
        const seq_t qn1_seq = (seq_t)qn1;

        /* qn⁴ and qn1⁴ */
        seq_t diff = qn_seq * qn_seq * qn_seq * qn_seq;
        diff -= qn1_seq * qn1_seq * qn1_seq * qn1_seq;

        int64_t diff_val = (int64_t)diff;   /* diff fits in 63 bits */
        sum += mu * diff_val;
    }

    return (seq_t)sum;   /* result is non‑negative for all n ≥ 1 */
}
