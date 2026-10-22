/*
 * A325369  Numbers with no two prime exponents appearing the same number of
 *          times in the prime signature.
 * https://oeis.org/A325369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers with distinct multiplicities of exponents in the prime
 *            signature (search algorithm)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: check whether a number satisfies the condition. */
static bool _is_valid(idx_t v) {
    idx_t tmp = v;
    /* Count how many primes have a given exponent.
       Exponents cannot exceed 63 for all numbers we handle. */
    int count_exp[64] = {0};

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint8_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            count_exp[exp]++;
        }
    }
    if (tmp > 1) {            /* remaining prime factor gives exponent 1 */
        count_exp[1]++;
    }

    /* Check that all non‑zero counts are distinct. */
    bool used_count[64] = {0};
    for (int e = 0; e < 64; ++e) {
        if (count_exp[e] > 0) {
            if (used_count[count_exp[e]])
                return false;
            used_count[count_exp[e]] = true;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    /* Sequence is 1‑based. */
    idx_t count = 0;
    idx_t cur   = 1;

    while (count < n) {
        if (_is_valid(cur)) {
            count++;
            if (count == n)
                return (seq_t)cur;
        }
        cur++;
    }
    /* Unreachable, but keep compiler happy. */
    return (seq_t)0;
}
