/*
 * A374110  a(n) = 1 if A113177(n) and A328845(n) are both multiples of 3,
 *          otherwise 0, where A113177 is fully additive with a(p) =
 *          Fibonacci(p) and A328845 is the first Fibonacci-based variant of
 *          the arithmetic derivative.
 * https://oeis.org/A374110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A113177(n) and A328845(n) are both multiples of 3,
 *            otherwise 0
 * Range:     u8 n <= 9156, u16 n <= 9156, u32 n <= 9156, u64 n <= 9156,
 *            u128 n <= 9156, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"

static const int FibMod3[8] = {0, 1, 1, 2, 0, 2, 2, 1};

/* Helper: compute f(p) mod 3 where f is Fibonacci */
static inline seq_t fib_mod3(uint64_t p)
{
    return (seq_t)FibMod3[p & 7];
}

/* Main function */
seq_t a(idx_t n)
{
    /* Static cache of primes up to the current limit */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;

    idx_t N = n;            /* keep original n for later use */

    /* Ensure we have primes up to sqrt(N) */
    idx_t need = _floor_isqrt_u64(N) + 1;
    if (need > primes_limit) {
        if (primes) free(primes);   /* release old table */
        build_primes_upto(need, &primes, &primes_cnt);
        primes_limit = need;
    }

    /* A113177(n) modulo 3  (fully additive, a(p)=Fib(p)) */
    seq_t mod113 = 0;
    idx_t tmp = N;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            mod113 += (seq_t)e * fib_mod3(p);
        }
    }
    if (tmp > 1) {                     /* remaining prime factor > sqrt */
        mod113 += fib_mod3(tmp);
    }
    mod113 %= 3;

    /* A328845(n) modulo 3  (Fibonacci‑based arithmetic derivative) */
    seq_t mod328 = 0;
    tmp = N;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            seq_t term = ((seq_t)e * fib_mod3(p)) % 3;
            term = (term * (seq_t)((N / p) % 3)) % 3;
            mod328 += term;
        }
    }
    if (tmp > 1) {
        uint64_t p = tmp;
        seq_t term = (fib_mod3(p)) % 3;                  /* e = 1 */
        term = (term * (seq_t)((N / p) % 3)) % 3;
        mod328 += term;
    }
    mod328 %= 3;

    return (mod113 == 0 && mod328 == 0) ? (seq_t)1 : (seq_t)0;
}
