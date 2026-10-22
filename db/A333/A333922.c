/*
 * A333922  a(n) = 1 - (floor(2^n*A002487(n)/A002487(n+1)) mod 2).
 * https://oeis.org/A333922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 - (floor(2^n*A002487(n)/A002487(n+1)) mod 2)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *s_cache = NULL;    /* Stern's diatomic sequence values */
static size_t s_size  = 0;       /* current allocated size of s_cache */

/* Fast exponentiation: compute 2^exp mod mod_val (mod_val > 0) */
static uint64_t pow2_mod(idx_t exp, uint64_t mod_val)
{
    uint64_t result = 1 % mod_val;
    uint64_t base   = 2 % mod_val;
    while (exp > 0)
    {
        if (exp & 1UL)
            result = (result * base) % mod_val;
        base = (base * base) % mod_val;
        exp >>= 1U;
    }
    return result;
}

/* Ensure Stern's sequence values up to index n+1 are computed and cached */
static void ensure_s_cache(idx_t n)
{
    /* Need values for indices 0 … n+1 */
    size_t needed = (size_t)n + 2;
    if (s_cache == NULL)
    {
        s_cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (s_cache == NULL) exit(1);
        s_cache[0] = (seq_t)0;
        if (needed > 1) s_cache[1] = (seq_t)1;
        s_size = needed;
    }
    else if (needed > s_size)
    {
        s_cache = (seq_t *)realloc(s_cache, needed * sizeof(seq_t));
        if (s_cache == NULL) exit(1);
        for (idx_t i = (idx_t)s_size; i < needed; ++i)
        {
            if (i < 2) continue; /* already initialized */
            if ((i & 1ULL) == 0ULL)  /* even index */
                s_cache[i] = s_cache[i >> 1];
            else                     /* odd index: 2k+1 */
                s_cache[i] = s_cache[i >> 1] + s_cache[(i >> 1) + 1];
        }
        s_size = needed;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute Stern's sequence up to n+1 if necessary */
    ensure_s_cache(n);

    seq_t sn  = s_cache[n];
    seq_t sn1 = s_cache[n + 1];

    /* Convert to 64‑bit for intermediate arithmetic */
    uint64_t b = (uint64_t)sn1;        /* divisor */
    uint64_t mod = b * 2U;            /* 2*b, always >0 since b>=1 for n>=0 */

    uint64_t pow2 = pow2_mod(n, mod);          /* 2^n mod (2*b) */
    uint64_t sn_mod = (uint64_t)sn % mod;      /* s[n] mod (2*b) */
    uint64_t m = (pow2 * sn_mod) % mod;        /* (2^n * s[n]) mod (2*b) */

    /* Parity of floor((2^n * s[n]) / s[n+1]) is 1 iff m >= b */
    uint64_t parity = (m >= b) ? 1ULL : 0ULL;

    return (seq_t)(1ULL - parity);
}
