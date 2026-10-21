/*
 * A279289  Numbers k such that phi(k) > tau(k).
 * https://oeis.org/A279289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A279289: numbers k such that phi(k) > tau(k)
 * First 19 terms are found by brute–force search.
 * For n ≥ 20 we use the proven linear formula a(n) = n + 11. */
#include <stdbool.h>

static seq_t phi_uint64(uint64_t n) {
    seq_t result = (seq_t)n;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / (seq_t)n;
    return result;
}

static seq_t tau_uint64(uint64_t n) {
    seq_t cnt = 1;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            cnt = cnt * (exp + 1);
        }
    }
    if (n > 1) cnt *= 2;
    return cnt;
}

static seq_t first_terms[19];
static bool initialized = false;

static void init_first_terms(void) {
    idx_t found = 0;
    uint64_t k = 2;
    while (found < 19) {
        seq_t phi = phi_uint64(k);
        seq_t tau = tau_uint64(k);
        if (phi > tau) {
            first_terms[found++] = (seq_t)k;
        }
        ++k;
    }
    initialized = true;
}

seq_t a(idx_t n) {
    if (n >= 20) return (seq_t)n + 11;
    if (!initialized) init_first_terms();
    return first_terms[n - 1];
}
