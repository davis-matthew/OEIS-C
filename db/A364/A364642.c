/*
 * A364642  a(n) is the number of iterations of psi(phi(x)) starting at x = n
 *          and terminating when psi(phi(x)) = x (n is counted), -1 otherwise.
 * https://oeis.org/A364642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 784, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = number of iterations of psi(phi(x)) starting at x = n
 * until the iteration reaches a fixed point (psi(phi(x)) = x).
 * n is counted as the first step.  Returns -1 if the iteration
 * never reaches a fixed point.  Works for all n >= 1. */

#include <stdint.h>

/* Dedekind ψ(k) = k * Π_{p|k} (1 + 1/p) */
static seq_t psi_of(seq_t x)
{
    uint64_t n   = (uint64_t)x;
    uint64_t res = n;
    if (n == 0) return 0;

    if ((n & 1) == 0) {                 /* even factor 2 */
        res = res * 3 / 2;              /* multiply by (2+1)/2 = 3/2 */
        while ((n & 1) == 0) n >>= 1;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            res = res * (p + 1) / p;     /* multiply by (p+1)/p */
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1)                           /* remaining prime factor */
        res = res * (n + 1) / n;
    return (seq_t)res;
}

/* Euler φ(k) : number of integers ≤ k coprime to k */
static seq_t phi_local(seq_t x)
{
    uint64_t n   = (uint64_t)x;
    uint64_t res = n;
    if (n == 0) return 0;

    if ((n & 1) == 0) {
        res = res / 2 * 1;               /* multiply by (2-1)/2 = 1/2 */
        while ((n & 1) == 0) n >>= 1;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            res = res / p * (p - 1);
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1)
        res = res / n * (n - 1);
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined offset – return -1 */
        return (seq_t)-1;

    seq_t current = n;              /* current value of x  */
    seq_t count   = 1;              /* n itself counts as step 1 */

    for (int i = 0; i < 1000; ++i) {
        seq_t v = phi_local(current);
        seq_t next = psi_of(v);
        if (next == current)
            return count;
        current = next;
        ++count;
    }
    return (seq_t)-1;               /* did not converge within limit */
}
