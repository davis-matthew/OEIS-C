/*
 * A338008  Odd composite integers m such that A001353(m)^2 == 1 (mod m).
 * https://oeis.org/A338008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search for odd composite m where U_{4,1}(m)^2 ≡ 1 (mod m) with
 *            U(0)=0, U(1)=1 and U(n+2)=4U(n+1)-U(n)
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

/* Generic modular multiplication:  (a*b) mod mod
   Works for any unsigned integer type that supports +, -, <<, >>, &. */
static seq_t mul_mod_seq(seq_t a, seq_t b, seq_t mod)
{
    seq_t result = 0;
    seq_t base   = a % mod;
    seq_t exp    = b;
    while (exp) {
        if (exp & (seq_t)1) {
            result = (result + base) % mod;
        }
        base = (base + base) % mod;
        exp >>= 1;
    }
    return result;
}

/* Compute U(n) modulo mod for the generalized Lucas sequence
   with parameters a=4, b=1. */
static seq_t U_mod(seq_t mod, idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1 % mod;

    /* Matrix exponentiation:  |4  -1|
                               |1   0|^(n-1) * [1;0] = [U(n);U(n-1)] */
    seq_t m[2][2];
    seq_t r[2][2];
    seq_t tmp[2][2];

    m[0][0] = 4 % mod;
    m[0][1] = (mod - 1) % mod;  /* -1 mod mod */
    m[1][0] = 1 % mod;
    m[1][1] = 0;

    r[0][0] = 1; r[0][1] = 0;
    r[1][0] = 0; r[1][1] = 1;

    idx_t e = n - 1;
    while (e) {
        if (e & (idx_t)1) {
            /* r = r * m */
            tmp[0][0] = (mul_mod_seq(r[0][0], m[0][0], mod) +
                         mul_mod_seq(r[0][1], m[1][0], mod)) % mod;
            tmp[0][1] = (mul_mod_seq(r[0][0], m[0][1], mod) +
                         mul_mod_seq(r[0][1], m[1][1], mod)) % mod;
            tmp[1][0] = (mul_mod_seq(r[1][0], m[0][0], mod) +
                         mul_mod_seq(r[1][1], m[1][0], mod)) % mod;
            tmp[1][1] = (mul_mod_seq(r[1][0], m[0][1], mod) +
                         mul_mod_seq(r[1][1], m[1][1], mod)) % mod;

            r[0][0] = tmp[0][0]; r[0][1] = tmp[0][1];
            r[1][0] = tmp[1][0]; r[1][1] = tmp[1][1];
        }
        /* m = m * m */
        tmp[0][0] = (mul_mod_seq(m[0][0], m[0][0], mod) +
                     mul_mod_seq(m[0][1], m[1][0], mod)) % mod;
        tmp[0][1] = (mul_mod_seq(m[0][0], m[0][1], mod) +
                     mul_mod_seq(m[0][1], m[1][1], mod)) % mod;
        tmp[1][0] = (mul_mod_seq(m[1][0], m[0][0], mod) +
                     mul_mod_seq(m[1][1], m[1][0], mod)) % mod;
        tmp[1][1] = (mul_mod_seq(m[1][0], m[0][1], mod) +
                     mul_mod_seq(m[1][1], m[1][1], mod)) % mod;

        m[0][0] = tmp[0][0]; m[0][1] = tmp[0][1];
        m[1][0] = tmp[1][0]; m[1][1] = tmp[1][1];

        e >>= 1;
    }

    /* U(n) = r[0][0] * U(1) + r[0][1] * U(0) = r[0][0] */
    return r[0][0] % mod;
}

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_to_check = 3; /* first odd composite candidate */

    /* Ensure cache has enough terms */
    if (cache_len < n) {
        grow_seq(&cache, &cache_cap, n);      /* enlarge if needed */
        while (cache_len < n) {
            /* Skip even numbers and numbers < 3 */
            if ((next_to_check & (idx_t)1) == 0 || next_to_check < 3) {
                ++next_to_check;
                continue;
            }
            /* Composite test */
            if (!is_prime(next_to_check)) {
                seq_t mod = (seq_t)next_to_check;
                seq_t u   = U_mod(mod, next_to_check);
                seq_t sq  = mul_mod_seq(u, u, mod);
                if (sq == (1 % mod)) {
                    cache[cache_len++] = mod;
                }
            }
            ++next_to_check;
        }
    }
    return cache[n - 1];
}
