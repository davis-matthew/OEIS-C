/*
 * A258847  Sum of the k-th pi-based arithmetic derivative of n-k for k=0..n.
 * https://oeis.org/A258847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A258850(n-k,k), where A258850(m,k) is the
 *            k‑th pi‑based arithmetic derivative of m
 * Range:     u8 n <= 12, u16 n <= 16, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   build_primes_upto, pi_derivative, pi_of_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/pi_derivative.h"
#include "../utils/pi_of_prime.h"
/* required utility prototypes */
extern void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out);

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Build a prime list large enough for all factors and pi(p) needed. */
    const idx_t prime_limit = 10000000;   /* 10⁷, enough for n ≤ 10⁴ */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(prime_limit, &primes, &prime_cnt);

    seq_t answer = 0;

    for (idx_t m = 0; m <= n; ++m) {
        seq_t val = (seq_t)m;      /* 0‑th derivative */
        idx_t offset = 0;

        while (true) {
            idx_t idx = m + offset;
            if (idx > n)
                break;

            if (idx == n)
                answer += val;

            if (val <= 1)
                break;                 /* further derivatives will be zero */

            seq_t next = pi_derivative(val, primes, prime_cnt);
            if (next == 0)
                break;

            val = next;
            ++offset;
        }
    }

    free(primes);
    return answer;
}
