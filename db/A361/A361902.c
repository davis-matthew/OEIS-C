/*
 * A361902  Least k such that n+A000045(k) is prime, or -1 if no such k
 *          exists.
 * https://oeis.org/A361902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the least k such that n + F_k is prime; a(n) = 0 iff n
 *            is prime; a(n) = -1 if n ≡ 14475 (mod 1647030)
 * Range:     i8 n <= 84, i16 n <= 7122, i32 n <= 7122, i64 n <= 7122,
 *            i128 n <= 7122, big any n
 * Validated: standard, pass, 103 terms through n = 7087 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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
    /* Check trivial prime case */
    if (is_prime_big((seq_t)n))
        return (seq_t)0;

    /* Forbidden residue class -> no such k */
    const idx_t M = 1647030ULL;
    if (n % M == 14475ULL)
        return (seq_t)-1;

    /* Iterate Fibonacci indices starting from 1 */
    seq_t f_prev = 0; /* F_0 */
    seq_t f_curr = 1; /* F_1 */
    for (idx_t k = 1; ; ++k) {
        seq_t candidate = (seq_t)n + f_curr;
        if (is_prime_big(candidate))
            return (seq_t)k;
        /* Next Fibonacci number: F_{k+1} = F_k + F_{k-1} */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
}
