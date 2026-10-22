/*
 * A361706  Inverse Moebius transform applied twice to primes.
 * https://oeis.org/A361706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A000005(n/d) * prime(d)
 * Range:     u8 n <= 23, u16 n <= 1259, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* where A000005 = τ(n) = number of divisors of n,
   and prime(k) = the k-th prime number. */

/* --------- divisor–count cache ------------------------------------- */
static idx_t max_tau_n = 0ull;
static idx_t *tau = NULL;

/* recompute τ(1)…τ(N) from scratch (N >= max_tau_n) */
static void ensure_tau(idx_t N)
{
    if (N <= max_tau_n) return;

    /* reallocate array to hold indices 0 … N */
    tau = (idx_t*)realloc(tau, (N + 1) * sizeof(idx_t));
    if (!tau) abort();

    /* reset all entries */
    for (idx_t i = 0; i <= N; ++i) tau[i] = 0ull;

    /* classic sieve‑like divisor counting */
    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t j = i; j <= N; j += i)
            ++tau[j];
    }
    max_tau_n = N;
}

/* ---------------- sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* sequence is defined for n ≥ 1 */

    ensure_tau(n);

    seq_t result = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;

            /* prime(d) and prime(d2) */
            seq_t pr1 = (seq_t)get_prime(d);
            seq_t pr2 = (seq_t)get_prime(d2);

            /* τ(n/d) * prime(d) */
            result += (seq_t)tau[n / d] * pr1;
            if (d2 != d)
                result += (seq_t)tau[n / d2] * pr2;
        }
    }
    return result;
}
