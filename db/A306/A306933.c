/*
 * A306933  Let k be any quadratic field such that all prime factors of n are
 *          inert in k, O_k be the corresponding ring of integers and G(n) =
 *          (O_k/nO_k)* be the multiplicative group of integers in O_k modulo
 *          n; then a(n) is the exponent of G(n).
 * https://oeis.org/A306933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm over all prime powers p^e dividing n of * a(p^e),
 *            where a(2^e) = 3 if e == 1 * = 3 * 2^(e-1) otherwise * and for
 *            odd p, a(p^e) = (p^2 - 1) * p^(e-1)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n by trial division (fast for n <= 10000),
 * computes each prime‑power contribution and keeps the running lcm.
 * It uses the provided gcd(seq_t, seq_t) function. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    idx_t     m   = n;      /* remaining part to factor */
    seq_t     res = (seq_t)1; /* running lcm */

    /* trial division for p = 2 ... sqrt(m) */
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            /* count exponent e of prime p */
            int e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }

            /* compute a(p^e) */
            seq_t val;
            if (p == 2)
            {
                seq_t pow = (seq_t)1;
                for (int i = 0; i < e - 1; ++i)
                    pow *= (seq_t)2;
                val = (e == 1) ? (seq_t)3 : (seq_t)3 * pow;
            }
            else
            {
                seq_t p_seq = (seq_t)p;
                seq_t p2     = p_seq * p_seq;
                seq_t base   = p2 - (seq_t)1;
                seq_t pow    = (seq_t)1;
                for (int i = 0; i < e - 1; ++i)
                    pow *= p_seq;
                val = base * pow;
            }

            /* update lcm: res = lcm(res, val) */
            seq_t g = gcd(res, val);
            res = (res / g) * val;
        }
    }

    /* if a prime factor > sqrt(original n) remains */
    if (m > 1)
    {
        idx_t p = m;   /* last prime factor */
        int e = 1;     /* exponent is 1 */

        seq_t val;
        if (p == 2)
        {
            val = (seq_t)3; /* e == 1 */
        }
        else
        {
            seq_t p_seq = (seq_t)p;
            seq_t p2     = p_seq * p_seq;
            val = p2 - (seq_t)1; /* (p^2 - 1) * p^(0) = p^2 - 1 */
        }

        seq_t g = gcd(res, val);
        res = (res / g) * val;
    }

    return res;
}
