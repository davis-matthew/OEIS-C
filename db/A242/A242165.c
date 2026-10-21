/*
 * A242165  Smallest k>=0, such that n+/-k are both Fermi-Dirac primes
 *          (A050376).
 * https://oeis.org/A242165
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k ≥ 0 such that n−k and n+k are both Fermi‑Dirac
 *            primes (numbers of the form p^(2^j), where p is prime)
 * Range:     u8 n <= 8410, u16 n <= 9739, u32 n <= 9739, u64 n <= 9739,
 *            u128 n <= 9739, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Return true iff base^exp ≤ limit (all values are unsigned 64‑bit). */
static bool power_leq(idx_t base, idx_t exp, idx_t limit) {
    __int128 res = 1;
    for (idx_t i = 0; i < exp; ++i) {
        res *= base;
        if (res > limit) return false;
    }
    return true;
}

/* Integer e‑th root: floor(n^(1/e)) for n ≥ 1, e ≥ 1. */
static idx_t int_root(idx_t n, idx_t e) {
    if (e == 1) return n;
    idx_t low = 1, high = n;            /* high always satisfies high^e ≥ n */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;   /* upper mid */
        if (power_leq(mid, e, n)) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* Return true iff n is a Fermi‑Dirac prime:
   n is prime, or n = p^(2^j) for some j≥1 with p prime.  */
static bool is_fd_prime(idx_t n) {
    if (n < 2) return false;
    if (is_prime(n)) return true;

    idx_t e = 2;
    while (e <= 64) {
        idx_t root = int_root(n, e);
        if (root <= 1) break;
        /* Compute root^e, guaranteed to fit in idx_t */
        __int128 val = 1;
        for (idx_t i = 0; i < e; ++i) val *= root;
        if ((idx_t)val == n && is_prime(root)) return true;
        e <<= 1;                        /* next power of two exponent */
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* n ≥ 2 by OEIS offset; undefined behavior for n < 2 is allowed */
    if (is_fd_prime(n)) return 0;

    idx_t k = 1;
    while (1) {
        if (k > n - 1) break;          /* should never happen for valid n */
        if (is_fd_prime(n - k) && is_fd_prime(n + k))
            return (seq_t)k;
        ++k;
    }

    /* Fallback (should never be reached) */
    return (seq_t)0;
}
