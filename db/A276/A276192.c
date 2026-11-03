/*
 * A276192  Numbers n such that there is no twin prime pair between A000217(n)
 *          and A000217(n+1) (n > 0).
 * https://oeis.org/A276192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the nth integer k ≥ 1 such that there is no twin prime
 *            pair p, p+2 with T_k ≤ p and p+2 < T_{k+1}, where T_k =
 *            k*(k+1)/2
 * Range:     u8 n <= 7, u16 n <= 35, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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

/* Return the nth term of OEIS A276192 */
seq_t a(idx_t n)
{
    idx_t   count = 0;       /* how many qualifying numbers found */
    for (idx_t k = 1; ; ++k) {
        /* triangular numbers in seq_t domain */
        seq_t K  = (seq_t)k;
        seq_t T_k   = K * (K + 1) / 2;
        seq_t T_k1  = (seq_t)(k + 1) * (seq_t)(k + 1 + 1) / 2;

        /* interval [T_k, T_k1-2] to scan for twin primes */
        seq_t start = T_k;
        seq_t end   = T_k1 - 2;   /* last possible p */

        bool has_twin = false;

        if (end >= start) {
            /* start with smallest odd p ≥ 3 not less than start */
            seq_t p = start;
            if (p < 3)
                p = 3; 
            if ((p & 1) == 0)
                ++p;    /* make odd */

            for (; p <= end; p += 2) {
                if (is_prime_big(p) && is_prime_big(p + 2)) {
                    has_twin = true;
                    break;
                }
            }
        }

        if (!has_twin) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
