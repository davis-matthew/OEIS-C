/*
 * A284148  Lexicographically earliest sequence of nonnegative integers such
 *          that a(1)=3 and the sequence is p-periodic mod p for any p > 0.
 * https://oeis.org/A284148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3. For n>1, a(n) is the lexicographically smallest
 *            non‑negative integer such that for all integers p with 2≤p≤n−1,
 *            a(n) ≡ a(n−p) (mod p)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 47, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The construction uses the general Chinese Remainder Theorem.  */

#include <stdlib.h>
#include <stdint.h>

/* --------- helper functions ------------------------------------- */

/* gcd for seq_t (unsigned arbitrary‑precision integers) */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* extended gcd for signed 64‑bit integers (used only for mod inverses) */
static int64_t egcd(int64_t a, int64_t b, int64_t *x, int64_t *y)
{
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a > 0 ? a : -a;
    }
    int64_t x1, y1;
    int64_t g = egcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

/* modular inverse of a modulo mod ( 1 <= a < mod ), returns 0 if not invertible */
static uint64_t modinv_uint64(uint64_t a, uint64_t mod)
{
    int64_t x, y;
    uint64_t g = (uint64_t)egcd((int64_t)a, (int64_t)mod, &x, &y);
    if (g != 1) return 0;
    if (x < 0) x += (int64_t)mod;
    return (uint64_t)x;
}

/* --------- sequence implementation -------------------------------- */

seq_t a(idx_t n)
{
    /*  cache for previously computed terms  */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* total capacity (indices 0..cap-1)  */
    static size_t cache_len = 0;     /* highest index actually filled   */

    if (n == 0) return 0;   /* undefined, but harmless */

    /* initialize cache */
    if (cache == NULL) {
        cache_cap = 2;            /* indices 0 and 1; we ignore index 0      */
        cache_len = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[1] = (seq_t)3;      /* a(1)=3                                 */
    }

    /* grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t idx = cache_len + 1; idx <= (size_t)n; ++idx) {
        seq_t x = 0;      /* current solution          */
        seq_t M = 1;      /* current modulus           */

        for (idx_t p = 2; p <= idx - 1; ++p) {
            seq_t r   = cache[(size_t)(idx - p)];  /* remainder a(idx-p) */
            seq_t m   = (seq_t)p;                  /* modulus p          */
            seq_t g   = seq_gcd(M, m);             /* gcd(M,m)           */

            /* Reduce all data to work modulo m and m' */
            seq_t a_mod_m = x % m;
            seq_t r_mod_m = r % m;
            seq_t diff_mod_m = (r_mod_m + m - a_mod_m) % m;   /* 0..m-1 */
            seq_t diff_div_g   = diff_mod_m / g;              /* < m/g */
            uint64_t rhs = (uint64_t)diff_div_g;              /* small value */

            seq_t m_prime = m / g;    /* m' = p/g   */
            uint64_t m_prime_uint = (uint64_t)m_prime;

            seq_t M_div_g = M / g;
            uint64_t M_div_g_mod = (uint64_t)(M_div_g % (seq_t)m_prime_uint);

            uint64_t inv = modinv_uint64(M_div_g_mod, m_prime_uint);
            /* gcd(M_div_g, m_prime) == 1, so inv exists */

            uint64_t k = (rhs * inv) % m_prime_uint;
            seq_t k_seq = (seq_t)k;

            seq_t newx = x + M * k_seq;
            seq_t newM  = M * m_prime;    /* lcm of M and m */

            x = newx % newM;      /* keep minimal non‑negative */
            M = newM;
        }
        cache[idx] = x;
    }
    cache_len = (size_t)n;
    return cache[(size_t)n];
}
