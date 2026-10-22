/*
 * A336019  a(n) is the smallest integer k (k>=2) such that 13...3 (1 followed
 *          by n 3's) mod k is even.
 * https://oeis.org/A336019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = smallest k ≥ 2 such that the integer 1 followed by n 3's
 *            (i.e. N_n = (4·10^n – 1)/3) has N_n mod k even
 * Range:     u8 n <= 9527, u16 n <= 9527, u32 n <= 9527, u64 n <= 9527,
 *            u128 n <= 9527, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: modular exponentiation of 10^exp mod modval using 128‑bit
   intermediate arithmetic to avoid overflow. */
static idx_t mod_pow10(idx_t exp, idx_t modval)
{
    idx_t result = 1 % modval;
    idx_t base   = 10 % modval;
    while (exp > 0)
    {
        if (exp & 1)
            result = (idx_t)(((unsigned __int128)result * base) % modval);
        base = (idx_t)(((unsigned __int128)base * base) % modval);
        exp >>= 1;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Iterate k from 2 upward until N_n mod k is even. */
    for (idx_t k = 2; ; ++k)
    {
        idx_t mod = 3 * k;                     /* 3k, multiple of 3 */
        idx_t ten_pow = mod_pow10(n, mod);     /* 10^n mod 3k */

        /* Compute (4·10^n – 1)/3 mod k. */
        idx_t t = (4 * ten_pow) % mod;         /* 4·10^n mod 3k */
        t = (t + mod - 1) % mod;               /* (4·10^n – 1) mod 3k */
        idx_t Nmodk = (t / 3) % k;             /* N_n mod k */

        if ((Nmodk & 1) == 0)                  /* even remainder */
            return (seq_t)k;
    }
}
