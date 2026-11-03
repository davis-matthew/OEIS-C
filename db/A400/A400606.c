/*
 * A400606  a(n) = least m >= 1 such that n*m^2 - 1 and n*m^2 + 1 are both
 *          prime, or 0 if there is no such m.
 * https://oeis.org/A400606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = least m ≥ 1 such that n*m^2 − 1 and n*m^2 + 1 are both
 *            prime * (0 if none). For perfect squares ≥ 9 the value is 0
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isqrt.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    /* Handle perfect squares ≥ 9 explicitly */
    if (n >= 9u) {
        idx_t t = isqrt(n);
        if (t * t == n) return 0;
    }

    seq_t n_seq = (seq_t)n;
    for (idx_t m = 1; ; ++m) {
        seq_t m_seq = (seq_t)m;
        seq_t m_sq  = m_seq * m_seq;         /* m^2 */
        seq_t val   = n_seq * m_sq;          /* n*m^2 */

        seq_t minus = val - 1;
        seq_t plus  = val + 1;

        /* Both numbers must be prime */
        if (is_prime_big(minus) && is_prime_big(plus))
            return (seq_t)m;
    }
    /* Control never reaches here, but to satisfy the compiler */
    return 0;
}
