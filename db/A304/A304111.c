/*
 * A304111  Partial sums of f(n) = (-1)^(1-A304109(n)).
 * https://oeis.org/A304111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*A304110(n) – n (A304110(n) = #{k ≤ n : A304109(k) = 1})
 * Range:     i8 n <= 9459, i16 n <= 9459, i32 n <= 9459, i64 n <= 9459,
 *            i128 n <= 9459, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* determine whether the binary polynomial of n (GF(2)[x]) is square‑free
   by testing gcd(p(x),p'(x)) = 1, where p'(x) is the formal derivative.   */

#include <stdint.h>
#include <stdbool.h>

/* degree of a binary polynomial represented by uint64_t   */
static int poly_degree(uint64_t a) {
    int d = -1;
    while (a) { d++; a >>= 1; }
    return d;
}

/* remainder of a modulo b, both polynomials over GF(2)          */
static uint64_t poly_mod(uint64_t a, uint64_t b) {
    int da = poly_degree(a);
    int db = poly_degree(b);
    while (da >= db && a != 0) {
        int shift = da - db;
        a ^= b << shift;
        da = poly_degree(a);
    }
    return a;
}

/* gcd of two polynomials over GF(2)                            */
static uint64_t poly_gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t r = poly_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

/* test whether the binary polynomial of n is square‑free        */
static bool is_sqfree(uint64_t n) {
    if (n == 1) return true;        /* constant polynomial 1 */
    if (n == 0) return false;       /* 0 has no derivative */

    /* derivative: keep bits with odd exponent, shift them down one position. */
    uint64_t deriv = (n >> 1) & 0x5555555555555555ULL;   /* mask 0101... */

    if (deriv == 0) return false;   /* derivative zero but n≠1 -> not square‑free */

    uint64_t g = poly_gcd(n, deriv);
    return g == 1;
}

/* main sequence function                                           */
seq_t a(idx_t n) {
    if (n == 0) return 0;

    idx_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (is_sqfree((uint64_t)i)) count++;
    }

    seq_t res = (seq_t)((seq_t)count * 2 - (seq_t)n);
    return res;
}
