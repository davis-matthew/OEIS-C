/*
 * A309756  Approximation of the 2-adic integer arctan(4) up to 2^n.
 * https://oeis.org/A309756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Σ_{i=0..⌊(n-3)/4⌋} (-1)^i * 4^(2i+1)/(2i+1)) mod 2^n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 34 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* includes */
#include <stdlib.h>
#include "../utils/mem.h"

/* Modular inverse of an odd integer a modulo 2^n (n >= 1).
   Uses Newton iteration: inv_{k+1} = inv_k * (2 - a*inv_k) mod 2^{k+1}. */
static seq_t modinv_odd(seq_t a, idx_t n)
{
    seq_t mod = (seq_t)2;          /* current modulus = 2^1 */
    seq_t inv = (seq_t)1;          /* inverse modulo 2^1 (a is odd => a ≡ 1 mod 2) */

    for (idx_t step = 1; step < n; ++step)
    {
        seq_t next_mod = mod * (seq_t)2;                 /* 2^(step+1) */

        /* Compute (2 - a * inv) modulo next_mod */
        seq_t prod = a * inv;
        seq_t rem  = prod % next_mod;
        /* (2 - rem) mod next_mod without negatives */
        seq_t mult = (next_mod - rem) + (seq_t)2;
        if (mult >= next_mod)
            mult -= next_mod;                            /* now mult < next_mod */

        inv = (inv * mult) % next_mod;
        mod = next_mod;
    }

    return inv;   /* inv ≡ a⁻¹ mod 2^n */
}

/* Forward declaration of helper: modular inverse of an odd integer modulo 2^n */
static seq_t modinv_odd(seq_t a, idx_t n);

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n < 3)
        return (seq_t)0;

    /* Compute modulus m = 2^n */
    seq_t mod = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        mod *= (seq_t)2;

    /* Determine upper bound for i */
    idx_t max_i = (n - 3) / 4;
    idx_t shift_max = 4 * max_i + 2;          /* highest power of two used (≤ n-1) */

    /* Precompute powers of two up to shift_max */
    size_t pow_len = (size_t)(shift_max + 1);
    seq_t *pow2 = mem_new(seq_t, pow_len);
    if (!pow2)
        return (seq_t)0;                       /* fallback on allocation failure */

    pow2[0] = (seq_t)1;
    for (size_t i = 1; i < pow_len; ++i)
        pow2[i] = pow2[i - 1] * (seq_t)2;

    /* Accumulate the series */
    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i <= max_i; ++i)
    {
        idx_t shift = 4 * i + 2;          /* exponent of two in 4^(2i+1) */
        seq_t term = pow2[shift];         /* 4^(2i+1) modulo 2^n (irrelevant beyond 2^n) */
        seq_t denom = (seq_t)(2 * i + 1); /* odd denominator */
        seq_t inv   = modinv_odd(denom, n); /* modular inverse modulo 2^n */

        term = (term * inv) % mod;        /* term modulo 2^n */

        if ((i & 1) == 0) /* even i : + sign */
            sum = (sum + term) % mod;
        else               /* odd i : - sign */
            sum = (sum + (mod - term)) % mod;
    }

    mem_free(pow2);
    return sum;
}
