/*
 * A347684  Array read by antidiagonals: T(n,k) (n>=1, k>=1) = f(n,k), where
 *          f(x,y) = x*red_inv(x,y) + y*red_inv(y,x) if gcd(x,y)=1, or 0 if
 *          gcd(x,y)>1, and red_inv is defined in the comments.
 * https://oeis.org/A347684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the antidiagonal coordinates of
 *            index n. For coprime i,j, T(i,j) = i*red_inv(i,j) +
 *            j*red_inv(j,i); otherwise T(i,j) = 0. red_inv(u,v) is the
 *            reduced inverse of u modulo v
 * Range:     u8 n <= 611, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean algorithm for seq_t (unsigned arbitrary‑precision integer) */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Modular inverse of u modulo v (gcd(u,v) must be 1).  Returns a value
   in [0, v-1] such that r*u ≡ 1 (mod v).  Uses only + - * / % on seq_t. */
static seq_t mod_inverse(seq_t u, seq_t v)
{
    seq_t a = u % v;           /* reduce u modulo v */
    seq_t b = v;
    seq_t x0 = (seq_t)1;
    seq_t x1 = (seq_t)0;
    seq_t orig_v = v;          /* keep original modulus for modulo reductions */

    while (b != (seq_t)0)
    {
        seq_t q = a / b;
        seq_t r = a % b;

        /* new_x = (x0 - q*x1) mod orig_v, kept positive */
        seq_t temp = (q * x1) % orig_v;
        seq_t new_x = (x0 + orig_v - temp) % orig_v;

        a = b;
        b = r;
        x0 = x1;
        x1 = new_x;
    }
    /* a is gcd; if not 1, return 0 (inv does not exist) */
    return (a == (seq_t)1) ? (x0 % orig_v) : (seq_t)0;
}

/* Reduced inverse: r in [0, v/2] with r*u ≡ ±1 (mod v) */
static seq_t red_inv(seq_t u, seq_t v)
{
    if (v == (seq_t)0)
        return (seq_t)0;      /* undefined, never happens for v>=1 */

    seq_t inv = mod_inverse(u, v);
    if (inv == (seq_t)0)
        return (seq_t)0;      /* u not coprime to v, but caller checks gcd first */

    seq_t half = v >> 1;      /* floor(v/2) */
    return (inv <= half) ? inv : (v - inv);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Locate antidiagonal coordinates (i,j) for index n */
    idx_t d = 1;
    while ((d * (d + 1) / 2) < n)
        d++;

    idx_t prev = (d - 1) * d / 2;
    idx_t offset = n - prev - 1;
    idx_t i = offset + 1;
    idx_t j = d - offset;

    seq_t u = (seq_t)i;
    seq_t v = (seq_t)j;

    /* If not coprime, value is 0 */
    if (seq_gcd(u, v) != (seq_t)1)
        return (seq_t)0;

    seq_t ru = red_inv(u, v);
    seq_t rv = red_inv(v, u);
    return u * ru + v * rv;
}
