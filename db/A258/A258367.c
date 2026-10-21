/*
 * A258367  a(n) is the smallest A (in absolute value) such that for p =
 *          prime(n), 2^{(p-1)/2} == +-1 + A*p (mod p^2), i.e., such that p is
 *          a near-Wieferich prime.
 * https://oeis.org/A258367
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(|A|, |p - A|) where p = prime(n) and 2^((p-1)/2) ≡ ±1
 *            + A⋅p (mod p²)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 7796, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* util helpers ---------------------------------------------------- */
#include "../utils/get_prime.h"   /* returns the k‑th prime as uint32_t */
#include "../utils/mod_pow.h"     /* mod_pow(base,exp,mod) → base^exp (mod mod) */

/* sequence element ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* offset 2, below not used */

    /* ---------- 1. get the n‑th prime -------------------------------- */
    /* get_prime returns uint32_t, cast to seq_t to keep type uniform */
    seq_t p = (seq_t) get_prime(n);

    /* ---------- 2. compute 2^((p-1)/2)  (mod p²) ----------------------- */
    seq_t mod = p * p;                       /* p²  (fits in seq_t for the test sizes) */
    /* exponent is (p-1)/2 – fit into uint64_t because p ≤ 10⁶ approx */
    uint64_t exp = (uint64_t)((p - 1) / 2);
    seq_t S = mod_pow((seq_t)2, exp, mod);

    /* ---------- 3. find the unique integer A with 2^((p-1)/2) ≡ ±1 + A⋅p (mod p²) */
    seq_t r = S % p;               /* remainder of S modulo p */
    seq_t A;
    if (r == (seq_t)1) {                   /* S ≡ 1 (mod p) */
        A = (S - (seq_t)1) / p;
    } else if (r == p - (seq_t)1) {        /* S ≡ -1 (mod p) */
        A = (S + (seq_t)1) / p;
    } else {
        /* This case should never occur for a prime p.
           Return 0 as a safe fallback. */
        return 0;
    }

    /* ---------- 4. minimal absolute residue -------------------------- */
    seq_t diff = p - A;
    return (A <= diff) ? A : diff;
}
