/*
 * A245608  Permutation of natural numbers, the even bisection of A245606
 *          halved: a(n) = A245606(2*n)/2.
 * https://oeis.org/A245608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048673(A245606(n)); A245606(1)=1, A245606(2m) = 1 +
 *            A003961(A245606(m)), A245606(2m+1) = A003961(1 + A245606(m));
 *            A048673(m) = (A003961(m)+1)/2
 * Range:     u8 n <= 22, u16 n <= 341, u32 n <= 5498, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 91 terms through n = 9648 (2026-11-07)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_prime.h"

/* A003961: replace each prime in the factorization by the next prime */
static seq_t A245608_s(seq_t m)
{
    if (m <= 1) return m;
    seq_t temp = m;
    seq_t res = 1;
    uint64_t p = 2;
    while (p <= 1000000 && (seq_t)p * (seq_t)p <= temp) {
        seq_t sp = (seq_t)p;
        if (temp % sp == 0) {
            uint64_t np = next_prime(p);
            while (temp % sp == 0) {
                temp /= sp;
                res *= (seq_t)np;
            }
        }
        p += (p == 2) ? 1 : 2;
    }
    if (temp > 1) {
        res *= (seq_t)next_prime((uint64_t)temp);
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    int top = 63;
    while (!((n >> top) & 1)) top--;
    seq_t v = 1;
    for (int i = top - 1; i >= 0; --i) {
        if ((n >> i) & 1)
            v = A245608_s(v + 1);
        else
            v = (seq_t)1 + A245608_s(v);
    }
    return (A245608_s(v) + 1) / 2;
}
