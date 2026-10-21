/*
 * A281190  Concatenation of the reversed digits of numbers from 1 to n, mod
 *          n.
 * https://oeis.org/A281190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base, easy
 * Formula:   a(n) = A138957(n) mod n (concatenation of reversed digits of
 *            1..n, modulo n)
 * Range:     u8 n <= 277, u16 n <= 9815, u32 n <= 9815, u64 n <= 9815,
 *            u128 n <= 9815, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Safe modular multiplication using 128‑bit intermediate.
   Works for any unsigned seq_t (uint8_t … uint64_t, unsigned __int128). */
static inline seq_t mod_mul_seq(seq_t a, seq_t b, seq_t mod)
{
    return ((__uint128_t)a * b) % mod;
}

/* Safe modular addition using 128‑bit intermediate. */
static inline seq_t add_mod_seq(seq_t a, seq_t b, seq_t mod)
{
    return ((__uint128_t)a + b) % mod;
}

/* Compute a(n) for the OEIS sequence A281190. */
seq_t a(idx_t n)
{
    seq_t mod = n;                 /* modulus */
    seq_t cur = 0;                 /* current value modulo n */

    for (idx_t j = 1; j <= n; ++j) {
        /* Reverse digits of j and count its length. */
        seq_t rev = 0;
        int   len = 0;
        idx_t t = j;
        do {
            rev = rev * 10 + (t % 10);
            t   /= 10;
            ++len;
        } while (t > 0);

        /* Compute 10^len mod n. */
        seq_t pow10 = 1;
        for (int k = 0; k < len; ++k)
            pow10 = mod_mul_seq(pow10, 10u, mod);

        /* cur = (cur * 10^len + rev) mod n. */
        cur = add_mod_seq(mod_mul_seq(cur, pow10, mod), rev, mod);
    }

    return cur;
}
