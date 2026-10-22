/*
 * A334929  Positive integers k such that there exists a positive integer m
 *          consisting of k identical digits and such that m is a multiple of
 *          k.
 * https://oeis.org/A334929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = kth positive integer k such that there exists a digit d
 *            (1 ≤ d ≤ 9) with k | d*(10^k-1)/9. The function searches k in
 *            increasing order, testing divisibility via modular
 *            exponentiation, and returns the nth such k
 * Range:     u8 n <= 45, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* modular exponentiation for 10^e mod mod.  e and mod fit in idx_t. */
static inline idx_t powmod10(idx_t e, idx_t mod)
{
    if (mod == 1) return 0;
    idx_t result = 1 % mod;
    idx_t base   = 10 % mod;
    while (e)
    {
        if (e & 1U)
            result = (idx_t)((unsigned __int128)result * base % mod);
        base = (idx_t)((unsigned __int128)base * base % mod);
        e >>= 1U;
    }
    return result;
}

/* storage of the sequence. */
static seq_t *s_cache = NULL;   /* array of terms (as seq_t) */
static size_t  s_cap   = 0;     /* allocated capacity */
static size_t  s_size  = 0;     /* number of terms stored */

/* ensure capacity for at least needed elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= s_cap) return;
    size_t new_cap = s_cap ? s_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_arr = (seq_t*)realloc(s_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();            /* fatal allocation error */
    s_cache = new_arr;
    s_cap   = new_cap;
}

/* Return the nth term of sequence A334929 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but harmless */

    /* grow the cache until we have at least n terms */
    if (s_size < (size_t)n)
    {
        ensure_capacity(n);
        /* start checking from the next k after the last stored one */
        idx_t k = s_size ? (idx_t) (s_cache[s_size-1]) + 1U : 1U;
        while (s_size < (size_t)n)
        {
            idx_t mod = 9 * k;                     /* 9*k */
            idx_t pow10 = powmod10(k, mod);        /* 10^k mod mod */
            idx_t rem = (pow10 + mod - 1U) % mod;   /* 10^k-1 mod mod */
            bool good = false;
            for (idx_t d = 1U; d <= 9U && !good; ++d)
                if (( (idx_t)((unsigned __int128)rem * d % mod) ) == 0U)
                    good = true;
            if (good)
            {
                s_cache[s_size++] = (seq_t)k;
            }
            ++k;
        }
    }
    return s_cache[n-1];
}
