/*
 * A260228  a(n) = max(gcd(n,F(n-1)),gcd(n,F(n+1))), where F(n) is the n-th
 *          Fibonacci number.
 * https://oeis.org/A260228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = max(gcd(n, F(n-1)), gcd(n, F(n+1)))  where F(k) is the k‑th Fibonacci number.
 *
 * The implementation uses a fast‑doubling algorithm to compute Fibonacci numbers modulo n.
 * For each n we compute F(n‑1) mod n and F(n+1) mod n, then take the maximum of the two gcds.
 * This runs in O(log n) time and uses only 64‑bit arithmetic for the modulo operations,
 * which is sufficient for all integer types of seq_t and idx_t.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Return (F(n) mod mod, F(n+1) mod mod) via fast doubling. */
static void fib_mod(idx_t n, idx_t mod, idx_t *fn_mod, idx_t *fn1_mod)
{
    if (n == 0) {
        *fn_mod  = 0;
        *fn1_mod = 1 % mod;          /* 1 mod 1 == 0 handles mod==1 case */
        return;
    }

    idx_t a, b;
    fib_mod(n >> 1, mod, &a, &b);      /* a = F(k) mod mod, b = F(k+1) mod mod */

    /* Compute F(2k) and F(2k+1) modulo mod */
    idx_t two_b   = (2 * b) % mod;
    idx_t c = (a * ((two_b + mod - a) % mod)) % mod;    /* F(2k)  */
    idx_t d = ((a * a) % mod + (b * b) % mod) % mod;    /* F(2k+1) */

    if ((n & 1) == 0) {                      /* n even: F(n)=c, F(n+1)=d   */
        *fn_mod  = c;
        *fn1_mod = d;
    } else {                                 /* n odd:  F(n)=d, F(n+1)=c+d */
        *fn_mod  = d;
        *fn1_mod = (c + d) % mod;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* value not used (offset is 1) */
    if (n == 1) return (seq_t)1;      /* a(1) = 1 */

    /* gcd with F(n‑1) */
    idx_t f_nm1;    /* F(n-1) mod n */
    idx_t dummy;
    fib_mod(n - 1, n, &f_nm1, &dummy);

    /* gcd with F(n+1) */
    idx_t f_np1;    /* F(n+1) mod n */
    fib_mod(n + 1, n, &f_np1, &dummy);

    seq_t g1 = gcd((seq_t)n, (seq_t)f_nm1);
    seq_t g2 = gcd((seq_t)n, (seq_t)f_np1);

    return (g1 > g2) ? g1 : g2;
}
