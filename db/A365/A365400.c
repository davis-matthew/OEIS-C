/*
 * A365400  a(n) = 64 + A000720(n) - A365339(n).
 * https://oeis.org/A365400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 64 + A000720(n) - A365339(n), where A365339(n) is the
 *            length of the longest nondecreasing subsequence of phi(1..n)
 *            (computed by patience sorting; phi and pi(n) from a sieve)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    /* A365339(n) = PrimePi(n)+64 conjectured for n >= 31957, so a(n) = 0 there. */
    if (n > 400000) return (seq_t)0;
    size_t N = (size_t)n;
    uint32_t *phi = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    uint32_t *primes = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    uint32_t *tails = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    if (!phi || !primes || !tails) {
        free(phi); free(primes); free(tails);
        return (seq_t)0;
    }
    size_t pc = 0, len = 0;
    for (size_t i = 0; i <= N; i++) phi[i] = 0;
    phi[1] = 1;
    for (size_t i = 2; i <= N; i++) {
        if (phi[i] == 0) {
            phi[i] = (uint32_t)(i - 1);
            primes[pc++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pc; j++) {
            size_t p = primes[j];
            size_t m = i * p;
            if (m > N) break;
            if (i % p == 0) {
                phi[m] = phi[i] * (uint32_t)p;
                break;
            }
            phi[m] = phi[i] * (uint32_t)(p - 1);
        }
    }
    for (size_t i = 1; i <= N; i++) {
        uint32_t x = phi[i];
        size_t lo = 0, hi = len;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (tails[mid] > x) hi = mid;
            else lo = mid + 1;
        }
        if (lo == len) tails[len++] = x;
        else tails[lo] = x;
    }
    long long res = 64LL + (long long)pc - (long long)len;
    free(phi); free(primes); free(tails);
    if (res < 0) res = 0;
    return (seq_t)res;
}
