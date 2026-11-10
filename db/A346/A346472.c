/*
 * A346472  a(n) = A011772(A276086(n)).
 * https://oeis.org/A346472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A011772(A276086(n)); computed by factoring A276086(n)
 *            from the primorial-base digits of n, then taking the smallest m
 *            >= 1 with m ≡ 0 or -1 (mod each prime-power part of
 *            2*A276086(n)), found by CRT over all subsets
 * Range:     u8 n <= 14, u16 n <= 57, u32 n <= 624, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Validated: standard, pass, 89 terms through n = 2016 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t q[40];
    int k = 0;
    seq_t M = 2;               /* M = 2*N */
    bool first = true;         /* prime 2 handled with extra factor */
    uint32_t primes[64];
    int np = 0;
    uint32_t cand = 2;
    idx_t rem = n;
    int idx = 0;

    q[k++] = 0; /* placeholder for 2-part */
    seq_t two_part = 2;

    while (true) {
        /* get idx-th prime */
        while (np <= idx) {
            bool ok = true;
            for (int i = 0; i < np; ++i) {
                if ((uint64_t)primes[i] * primes[i] > cand) break;
                if (cand % primes[i] == 0) { ok = false; break; }
            }
            if (ok) primes[np++] = cand;
            cand++;
        }
        uint32_t p = primes[idx];
        if (rem == 0) break;
        uint32_t d = (uint32_t)(rem % p);
        rem /= p;
        if (d > 0) {
            if (first) {
                for (uint32_t j = 0; j < d; ++j) two_part *= 2;
            } else {
                seq_t pw = 1;
                for (uint32_t j = 0; j < d; ++j) pw *= (seq_t)p;
                q[k++] = pw;
                M *= pw;
            }
        }
        first = false;
        idx++;
    }
    q[0] = two_part;
    M *= (two_part / 2);

    seq_t best = M;
    unsigned long long full = (1ULL << k) - 1;
    for (unsigned long long mask = 0; mask < full; ++mask) {
        seq_t A = 1, B = 1;
        for (int i = 0; i < k; ++i) {
            if ((mask >> i) & 1) A *= q[i];
            else B *= q[i];
        }
        /* B > 1 here */
        seq_t r0 = B, r1 = A % B;
        seq_t u0 = 0, u1 = 1;
        int step = 1;
        while (r1 != 1) {
            seq_t qq = r0 / r1;
            seq_t r2 = r0 - qq * r1;
            seq_t u2 = u0 + qq * u1;
            r0 = r1; r1 = r2;
            u0 = u1; u1 = u2;
            step++;
        }
        seq_t inv = (step & 1) ? u1 : (B - u1);
        seq_t t = (B - inv) % B;
        seq_t m = A * t;
        if (m != 0 && m < best) best = m;
    }
    return best;
}
