/*
 * A270414  Numbers m such that sigma(m-1) and sigma(phi(m)) are both primes.
 * https://oeis.org/A270414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   append_candidate, calc_phi, is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A270414: numbers m such that sigma(m-1) and sigma(phi(m)) are primes (search algorithm) */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"
#include "../utils/calc_phi.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t count = 0;
    static size_t cap = 0;
    static idx_t last_checked = 1;

    while (count < n) {
        last_checked++;
        idx_t m = last_checked;

        seq_t s1 = sigma_of(m - 1);
        if (!is_prime_big(s1))
            continue;

        seq_t phi_seq = calc_phi((seq_t)m);
        idx_t phi = (idx_t)phi_seq;
        seq_t s2 = sigma_of(phi);
        if (!is_prime_big(s2))
            continue;

        append_candidate(&terms, &count, &cap, m);
    }
    return terms[n - 1];
}
