/*
 * A326416  The numbers k for which gcd(k, phi(k)) + gcd(k, tau(k)) = gcd(k,
 *          sigma(k)).
 * https://oeis.org/A326416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by brute-force search over integers k. Returns the nth
 *            integer k (n ≥ 1) such that gcd(k,phi(k)) + gcd(k,tau(k)) ==
 *            gcd(k,sigma(k))
 * Range:     u8 none, u16 n <= 3, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-03)
 * Helpers:   gcd, is_prime, mem, num_divisors, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"
#include "../utils/num_divisors.h"
#include "../utils/totient.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n < 1 */

    /* Static cache of found terms. */
    static seq_t *seq_list = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t next_k = 2;         /* next integer to test */

    while (seq_len < (size_t)n) {
        idx_t k = next_k++;

        /* Skip primes – they cannot satisfy the equation. */
        if (is_prime(k))
            continue;

        /* Compute required arithmetic functions. */
        seq_t phi_k   = totient(k);
        seq_t sigma_k = sigma_of(k);
        uint64_t tau_k = num_divisors(k);

        seq_t g1 = gcd((seq_t)k, phi_k);
        seq_t g2 = gcd((seq_t)k, (seq_t)tau_k);
        seq_t g3 = gcd((seq_t)k, sigma_k);

        if (g1 + g2 == g3) {
            /* Expand cache if necessary. */
            if (seq_len + 1 > seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq_list = mem_resize(seq_t, seq_list, seq_cap);
            }
            seq_list[seq_len++] = (seq_t)k;
        }
    }
    return seq_list[(size_t)(n - 1)];
}
