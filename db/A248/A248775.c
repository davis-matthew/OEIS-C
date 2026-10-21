/*
 * A248775  Greatest 7th-power-free divisor of n!.
 * https://oeis.org/A248775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{p prime ≤ n} p^{ e_p mod 7 } , where e_p =
 *            Σ_{k≥1} ⌊n / p^k⌋ is the exponent of p in n!
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the greatest 7‑th‑power‑free divisor of n!.  */

#include <stdbool.h>
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */

static bool is_prime(idx_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t p = 2; p <= n; ++p) {
        if (!is_prime(p)) continue;
        idx_t exp = 0;
        idx_t t = n / p;
        while (t) {
            exp += t;
            t /= p;
        }
        idx_t r = exp % 7;
        if (r == 0) continue;
        seq_t powp = seq_pow((seq_t)p, r);
        result *= powp;
    }
    return result;
}
