/*
 * A253554  a(1) = 1, a(2n) = n, a(2n+1) = A250470(2n+1).
 * https://oeis.org/A253554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2n)=n, a(2n+1)=A250470(2n+1): for odd n>=3 with
 *            p=spf(n), r=A078898(n), a(n) = r-th number k with
 *            spf(k)=prevprime(p) (even numbers if p=3). Computed by direct
 *            counting/search
 * Range:     u8 n <= 262, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 103 terms through n = 8002 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool spf_is(idx_t k, idx_t q)
{
    /* true iff k is a multiple of q with no divisor in [2,q) */
    for (idx_t d = 2; d < q && d * d <= k; ++d)
        if (k % d == 0) return false;
    return true;
}

static bool small_is_prime(idx_t x)
{
    if (x < 2) return false;
    for (idx_t d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n % 2 == 0) return (seq_t)(n / 2);

    /* smallest prime factor of n */
    idx_t p = n;
    for (idx_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) { p = d; break; }
    }

    /* rank of n among numbers with smallest prime factor p */
    idx_t r = 0;
    for (idx_t k = p; k <= n; k += p)
        if (spf_is(k, p)) ++r;

    /* previous prime */
    idx_t q = p - 1;
    while (!small_is_prime(q)) --q;

    if (q == 2) return (seq_t)(2 * r);

    idx_t cnt = 0;
    for (idx_t k = q;; k += q) {
        if (spf_is(k, q)) {
            ++cnt;
            if (cnt == r) return (seq_t)k;
        }
    }
}
