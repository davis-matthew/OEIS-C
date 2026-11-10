/*
 * A254116  Permutation of natural numbers: a(n) = A064216(A254103(n)).
 * https://oeis.org/A254116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A064216(A254103(n)); A254103(0)=0,
 *            A254103(2n)=3*A254103(n)-1,
 *            A254103(2n+1)=floor(3*(1+A254103(n))/2);
 *            A064216(m)=A064989(2m-1) replaces each odd prime p by
 *            prevprime(p)
 * Range:     u8 n <= 64, u16 n <= 2048, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 103 terms through n = 8177 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A254116_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if ((x & 1) == 0) return false;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A254116_prevprime(uint64_t p)
{
    for (uint64_t q = p - 1; q >= 2; --q)
        if (A254116_isprime(q)) return q;
    return 2;
}

seq_t a(idx_t n)
{
    /* A254103(n) via bits of n, top bit first */
    int top = 63;
    while (top >= 0 && !((n >> top) & 1)) --top;
    uint64_t v = 0;
    for (int i = top; i >= 0; --i) {
        if ((n >> i) & 1) v = (3 * (v + 1)) / 2;
        else v = 3 * v - 1;
    }

    /* A064216(v) = A064989(2v-1) */
    uint64_t t = 2 * v - 1;
    seq_t result = 1;
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            uint64_t prev = A254116_prevprime(p);
            while (t % p == 0) {
                t /= p;
                result *= (seq_t)prev;
            }
        }
    }
    if (t > 1) result *= (seq_t)A254116_prevprime(t);
    return result;
}
