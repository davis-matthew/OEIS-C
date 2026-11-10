/*
 * A366788  a(n) = A366388(A163511(n)).
 * https://oeis.org/A366788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A366388(A163511(n)); A366388 is totally additive with
 *            f(2)=0, f(prime(k)) = 1 + f(k) for k>1. A163511(n) = prime(r) *
 *            Π prime(i)^e_i, so a(n) = g(r) + Σ e_i*g(i), where g(k) =
 *            f(prime(k))
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9982 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned long long A366388_small(unsigned long long k);

/* f(prime(k)) */
static unsigned long long A366388_g(unsigned long long k)
{
    if (k == 1) return 0;
    return 1 + A366388_small(k);
}

/* index of prime p (p small) */
static unsigned long long A366388_pi(unsigned long long p)
{
    unsigned long long c = 0;
    for (unsigned long long q = 2; q <= p; q++) {
        int pr = 1;
        for (unsigned long long d = 2; d * d <= q; d++)
            if (q % d == 0) { pr = 0; break; }
        if (pr) c++;
    }
    return c;
}

/* f(k) for small k */
static unsigned long long A366388_small(unsigned long long k)
{
    unsigned long long total = 0;
    for (unsigned long long p = 2; k > 1; p++) {
        while (k % p == 0) {
            if (p != 2) total += 1 + (A366388_pi(p) == 1 ? 0 : 0) + 0, total += 0;
            if (p != 2) {
                total -= 1;
                total += A366388_g(A366388_pi(p));
            }
            k /= p;
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return (seq_t)0;
    idx_t r = 0, prev = 0, bitpos = 1, tmp = n;
    unsigned long long idxs[64];
    unsigned long long exps[64];
    while (tmp) {
        if (tmp & 1ULL) {
            exps[r] = (unsigned long long)(bitpos - prev - 1);
            r++;
            prev = bitpos;
        }
        bitpos++;
        tmp >>= 1;
    }
    (void)idxs;
    seq_t result = (seq_t)A366388_g((unsigned long long)r);
    for (idx_t i = 0; i < r; i++) {
        if (exps[i])
            result += (seq_t)exps[i] * (seq_t)A366388_g((unsigned long long)(i + 1));
    }
    return result;
}
