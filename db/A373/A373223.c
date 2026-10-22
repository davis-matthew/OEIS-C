/*
 * A373223  Gauss's triangle read by rows: T(n, k) = KP(n, k) * KP(k, n) where
 *          KP(n, k) = KroneckerSymbol(prime(n), prime(k)). (Law of Quadratic
 *          Reciprocity.)
 * https://oeis.org/A373223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = Kronecker(prime(row),prime(col)) *
 *            Kronecker(prime(col),prime(row)) where (row,col) are the 1‑based
 *            coordinates of n in the lower‑triangular array read by rows
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- prime storage ------------------------------------- */
static uint64_t *primes = NULL;   /* 0‑based array of primes */
static idx_t    prime_cnt = 0;    /* number of primes stored */

/* ensure that primes[0 .. need-1] exist */
static void ensure_primes(idx_t need)
{
    if (need <= prime_cnt) return;
    uint64_t cand = (prime_cnt == 0) ? 2ULL : primes[prime_cnt - 1] + 1ULL;
    while (prime_cnt < need)
    {
        bool isprime = true;
        for (idx_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { isprime = false; break; }
        }
        if (isprime)
        {
            uint64_t *tmp = (uint64_t *)realloc(primes,
                                                sizeof(uint64_t) * (prime_cnt + 1));
            if (!tmp) abort();          /* out of memory */
            primes = tmp;
            primes[prime_cnt++] = cand;
        }
        ++cand;
    }
}

/* ---------- modular exponentiation for 64‑bit modulus  -------- */
static uint64_t mod_pow_uint(uint64_t base, idx_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (exp)
    {
        if (exp & 1ULL) result = (uint64_t)((__int128)result * base % mod);
        base = (uint64_t)((__int128)base * base % mod);
        exp >>= 1ULL;
    }
    return result;
}

/* ---------- Legendre symbol (a/p), p odd prime ---------------- */
static seq_t legendre(uint64_t a, uint64_t p)
{
    uint64_t a_mod = a % p;
    if (a_mod == 0) return 0;
    uint64_t r = mod_pow_uint(a_mod, (p - 1) / 2, p);
    if (r == 1)     return 1;
    if (r == p - 1) return -1;
    return 0;      /* never reached for prime p */
}

/* ---------- Kronecker symbol for odd primes or 2 ---------------- */
static seq_t kronecker(uint64_t a, uint64_t b)
{
    if (a == b) return 0;               /* (p/p)=0 */
    if (a == 2ULL) {                    /* (2/b) */
        uint64_t m = b % 8ULL;
        return (m == 1ULL || m == 7ULL) ? 1 : -1;
    }
    if (b == 2ULL) {                    /* (a/2) */
        uint64_t m = a % 8ULL;
        return (m == 1ULL || m == 7ULL) ? 1 : -1;
    }
    /* both odd primes, distinct */
    return legendre(a, b);
}

/* ---------- main sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1ULL) return 0; /* undefined for n == 0, harmless */

    /* find row: smallest r with r*(r+1)/2 >= n */
    long double r_ld = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t row = (idx_t)((r_ld - 1.0L) / 2.0L);          /* floor value */
    while ((unsigned __int128)row * (row + 1ULL) / 2ULL < n) ++row;
    while (row > 0 &&
           (unsigned __int128)(row - 1ULL) * row / 2ULL >= n) --row;

    unsigned __int128 prev_tri = (unsigned __int128)(row - 1ULL) * row / 2ULL;
    idx_t col = (idx_t)(n - (idx_t)prev_tri);         /* 1‑based column */

    /* ensure required primes are available */
    idx_t need = (row > col) ? row : col;
    ensure_primes(need);

    uint64_t p_row = primes[row - 1U];
    uint64_t p_col = primes[col - 1U];

    seq_t s1 = kronecker(p_row, p_col);
    seq_t s2 = kronecker(p_col, p_row);

    return s1 * s2;   /* product ∈ {‑1,0,1} */
}
