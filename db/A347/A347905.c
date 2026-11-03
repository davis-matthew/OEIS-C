/*
 * A347905  Array read by antidiagonals, m, n >= 1: T(m,n) is the position of
 *          the first prime (after the two initial terms) in the
 *          Fibonacci-like sequence with initial terms m and n, or 0 if no
 *          such prime exists.
 * https://oeis.org/A347905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,n) where T(m,n) is defined recursively: * T(m,n) = 0
 *            if gcd(m,n) > 1; * T(m,n) = 2 if m + n is prime; * T(m,n) = T(n,
 *            m+n) + 1 otherwise. * The pair (m,n) is obtained by reading the
 *            infinite matrix by * antidiagonals. The function handles any n
 *            >= 1 and memoizes * results internally to...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-03)
 * Helpers:   gcd, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
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

seq_t a(idx_t n)
{
    /* determine the antidiagonal coordinates (m, k) for index n */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n) {
        d++;                      /* find smallest d with d(d+1)/2 >= n */
    }
    idx_t prev = (d - 1) * d / 2;  /* number of terms before diagonal d */
    idx_t pos  = n - prev;        /* position inside the diagonal (1‑based) */
    seq_t m = (seq_t)pos;
    seq_t k = (seq_t)(d + 1 - pos);  /* second coordinate */

    /* compute T(m,k) */
    if (gcd(m, k) != 1) {
        return (seq_t)0;
    }

    seq_t a0 = m;
    seq_t a1 = k;
    seq_t cnt = 2;                /* position of a0 + a1 in the sequence */
    seq_t sum = a0 + a1;

    if (is_prime_big(sum)) {
        return cnt;
    }

    /* iterate until a prime is found or a safe limit is reached */
    const seq_t LIMIT = 100000;
    while (cnt < LIMIT) {
        a0 = a1;
        a1 = sum;
        cnt++;
        sum = a0 + a1;
        if (is_prime_big(sum)) {
            return cnt;
        }
    }

    /* no prime found within the safe limit */
    return (seq_t)0;
}
