/*
 * A395009  a(n) is the greatest k < n such that {phi*k} < {phi*n}, where {}
 *          denotes the fractional part and phi = A001622.
 * https://oeis.org/A395009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Computes a(n) as the greatest k < n with {φ k} < {φ n}
   using a rational approximation of φ via Fibonacci convergents. */

#include <stdbool.h>

/* Modular multiplication that works for 64–bit seq_t and for
   arbitrary-precision seq_t. */
static inline seq_t modmul(seq_t a, seq_t b, seq_t mod)
{
#if defined(__SIZEOF_INT128__)
    return (unsigned __int128)a * b % mod;
#else
    seq_t res = 0;
    a %= mod;
    while (b)
    {
        if (b & 1) {
            res = (res + a) % mod;
        }
        a = (a << 1) % mod;
        b >>= 1;
    }
    return res;
#endif
}

/* Returns the greatest k < n such that {φ k} < {φ n}. */
static seq_t approx_phi_n(idx_t n)    /* helper: fractional part via rational */
{
    /* static approximation of φ = F_{k+1}/F_k with large denominator */
    static seq_t P = 0;   /* numerator */
    static seq_t Q = 0;   /* denominator */
    static bool initialized = false;
    if (!initialized)
    {
        /* Compute successive Fibonacci numbers until denominator Q
           exceeds 2·10^18, ensuring 64‑bit seq_t compatibility. */
        seq_t a = 1, b = 1;  /* a = F_k, b = F_{k+1} */
        const seq_t LIM = (seq_t)2000000000000000000ULL; /* 2×10^18 */
        while (a <= LIM)
        {
            seq_t tmp = a + b; /* F_{k+2} */
            a = b;            /* F_{k+1} */
            b = tmp;          /* F_{k+2} */
        }
        Q = a;  /* F_k > LIM */
        P = b;  /* F_{k+1} */
        initialized = true;
    }
    return modmul((seq_t)n, P, Q);
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;   /* no k < n */

    seq_t rn = approx_phi_n(n);

    for (idx_t k = n - 1; k > 0; --k)
    {
        seq_t rk = approx_phi_n(k);
        if (rk < rn)
            return (seq_t)k;
    }
    return 0;   /* default fallback, should not occur */
}
