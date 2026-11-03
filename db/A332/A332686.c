/*
 * A332686  a(n) = Sum_{k=1..n} phi(k/gcd(n, k)).
 * https://oeis.org/A332686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=1}^{n} \phi(k / \gcd(n,k))
 * Range:     u8 n <= 30, u16 n <= 466, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9919 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A332686 for all n >= 1. */
/* Uses a simple phi sieve up to n on each call. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Allocate array for Euler's totient values up to n. */
    seq_t *phi = mem_new(seq_t, (n + 1));
    if (!phi) abort();

    /* Initialise phi[i] = i. */
    for (idx_t i = 0; i <= n; ++i) phi[i] = (seq_t)i;

    /* Compute phi values via the classic sieve. */
    for (idx_t i = 2; i <= n; ++i) {
        if (phi[i] == (seq_t)i) {          /* i is prime */
            for (idx_t j = i; j <= n; j += i) {
                phi[j] -= phi[j] / (seq_t)i;
            }
        }
    }

    /* Accumulate the required sum. */
    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t d = gcd(k_seq, n_seq);
        seq_t m = k_seq / d;                     /* m = k / gcd(n,k) */
        sum += phi[(idx_t)m];
    }

    mem_free(phi);
    return sum;
}
