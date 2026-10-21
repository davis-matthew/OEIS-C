/*
 * A268491  Array T(n,k) = least integer congruent to prime(i) mod prime(i+1)
 *          for all k <= i <= k+n; n, k >= 1; read by upward diagonals.
 * https://oeis.org/A268491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(d-k, k+1) where * d is the smallest integer with
 *            d*(d+1)/2 >= n, * k = n - (d-1)*d/2 - 1 (zero‑based offset in
 *            the diagonal). * T(n,k) is the least integer x with * x ≡
 *            prime(i) (mod prime(i+1)) for all i in [k, k+n). * This function
 *            implements the definition by successive Chinese‑Remain...
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------------------------------------------------------- */
/*  Prime table (generated on demand)                         */
/* ---------------------------------------------------------- */
static uint64_t *primes = NULL;   /* array of primes, zero‑based: primes[0]=2 */
static size_t   prime_cnt = 0;

/* Simple trial‑division prime generation up to required amount. */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need) return;

    size_t new_cap = prime_cnt ? prime_cnt * 2 : 16;
    while (new_cap < need) new_cap *= 2;

    primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));

    uint64_t cand = (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 1;
    for (; prime_cnt < need; ++cand)
    {
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= cand; ++i)
        {
            if (cand % primes[i] == 0) { is_prime = 0; break; }
        }
        if (is_prime) primes[prime_cnt++] = cand;
    }
}

/* ---------------------------------------------------------- */
/*  Modular inverse modulo a 64‑bit prime (extended GCD)      */
/* ---------------------------------------------------------- */
static int64_t egcd(int64_t a, int64_t b, int64_t *x, int64_t *y)
{
    if (b == 0) { *x = 1; *y = 0; return a; }
    int64_t x1, y1;
    int64_t g = egcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

static uint64_t modinv_uint64(uint64_t a, uint64_t mod)
{
    int64_t x, y;
    int64_t g = egcd((int64_t)a, (int64_t)mod, &x, &y);
    (void)g; /* assumed prime, so g==1 */
    int64_t res = x % (int64_t)mod;
    if (res < 0) res += mod;
    return (uint64_t)res;
}

/* ---------------------------------------------------------- */
/*  Find diagonal index d such that d*(d+1)/2 >= n             */
/* ---------------------------------------------------------- */
static idx_t diag_of_idx(idx_t n)
{
    idx_t low = 1, high = 1;
    while ( (high * (high + 1u)) / 2u < n ) high <<= 1;
    while ( low < high )
    {
        idx_t mid = low + (high - low) / 2;
        if ( (mid * (mid + 1u)) / 2u >= n ) high = mid;
        else                               low = mid + 1;
    }
    return low;
}

/* ---------------------------------------------------------- */
/*  Main function                                               */
/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* 1. Locate the position (row, col) in T(n,k) along the diagonal. */
    idx_t d   = diag_of_idx(n);                           /* diagonal number */
    idx_t prev = (d - 1u) * d / 2u;                       /* preceding total count */
    idx_t k0  = n - prev - 1u;                             /* offset in this diagonal (zero‑based) */
    idx_t nlen = d - k0;                                   /* number of congruences (length of T) */
    idx_t kcol = k0 + 1u;                                 /* column index, 1‑based */

    /* 2. Ensure we have enough primes for indices up to kcol + nlen. */
    size_t need = (size_t)(kcol + nlen);
    ensure_primes(need);

    /* 3. Chinese Remainder: start with the first congruence. */
    seq_t r = (seq_t)primes[kcol - 1u];       /* residue prime(kcol)   */
    seq_t M = (seq_t)primes[kcol];           /* modulus prime(kcol+1) */

    /* 4. Merge the remaining congruences. */
    for (idx_t t = 1u; t < nlen; ++t)
    {
        /* Current prime indices: i = kcol + t  (1‑based) */
        uint64_t s   = primes[(kcol + t) - 1u]; /* prime(i)           */
        uint64_t mod = primes[(kcol + t)    ]; /* prime(i+1)         */

        /* r and M modulo the new modulus. */
        uint64_t r_mod = (uint64_t)(r % (seq_t)mod);
        uint64_t m_mod = (uint64_t)(M % (seq_t)mod);

        /* Compute the adjustment needed. */
        uint64_t delta_mod = (s + mod - r_mod) % mod;

        /* Inverse of m_mod modulo mod. */
        uint64_t inv = modinv_uint64(m_mod, mod);

        uint64_t t_mul = (delta_mod * inv) % mod;

        /* Update the solution. */
        r = r + M * (seq_t)t_mul;
        M = M * (seq_t)mod;
    }

    return r;
}
