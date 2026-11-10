/*
 * A245708  Permutation of natural numbers, the odd bisection of A245606
 *          incremented by one and halved: a(n) = (1+A245606((2*n)-1))/2.
 * https://oeis.org/A245708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = A048673(1+A245606(n-1)) for n > 1, where
 *            A048673(k) = (A003961(k)+1)/2; A245606(1)=1,
 *            A245606(2m)=1+A003961(A245606(m)),
 *            A245606(2m+1)=A003961(1+A245606(m))
 * Range:     u8 n <= 29, u16 n <= 343, u32 n <= 7677, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 91 terms through n = 9893 (2026-11-07)
 * Helpers:   build_primes_upto, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/next_prime.h"

static uint64_t *A245708_pr = NULL;
static size_t A245708_cnt = 0;
static bool A245708_ready = false;

/* A003961: shift prime factorization to next primes */
static seq_t A245708_s(seq_t n)
{
    if (!A245708_ready) {
        build_primes_upto(100000, &A245708_pr, &A245708_cnt);
        A245708_ready = true;
    }
    if (n <= 1) return n;
    seq_t t = n;
    seq_t res = 1;
    for (size_t i = 0; i < A245708_cnt; ++i) {
        seq_t p = (seq_t)A245708_pr[i];
        if (p > t / p) break;
        if (t % p == 0) {
            seq_t np = (seq_t)next_prime(A245708_pr[i]);
            while (t % p == 0) {
                t /= p;
                res *= np;
            }
        }
    }
    if (t > 1) {
        res *= (seq_t)next_prime((uint64_t)t);
    }
    return res;
}

/* A245606 computed iteratively from the binary expansion of m */
static seq_t A245708_A245606(idx_t m)
{
    int top = 0;
    for (int i = 63; i >= 0; --i) {
        if ((m >> i) & 1) { top = i; break; }
    }
    seq_t v = 1;
    for (int i = top - 1; i >= 0; --i) {
        if ((m >> i) & 1)
            v = A245708_s(v + 1);
        else
            v = A245708_s(v) + 1;
    }
    return v;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t x = A245708_A245606(n - 1) + 1;
    return (A245708_s(x) + 1) / 2;
}
