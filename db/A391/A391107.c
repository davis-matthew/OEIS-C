/*
 * A391107  Integers k such that the equation x^2 + y^2 = 2*k^2 has at least 4
 *          solutions in positive integers x < y.
 * https://oeis.org/A391107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A391107 – Numbers k such that x² + y² = 2·k² has at least
    4 solutions with 0 < x < y.  Equivalently,
    product_{p≡1 (mod 4)} (2·e_p+1)  ≥ 9, where k = ∏p^e_p.
    The function returns the nth term (a(1) = 65).  The sequence is
    generated lazily and cached so that calls in any order are O(1)
    after the term has been produced.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Check whether a candidate k satisfies the prime‑product condition   */
/* --------------------------------------------------------------------- */
static bool satisfies(uint64_t k)
{
    uint64_t n = k;
    uint64_t prod = 1;

    /* ignore factors of 2 – they do not appear in the product   */
    while ((n & 1ULL) == 0)
        n >>= 1;

    /* trial division up to √n (sufficient for our size range)   */
    for (uint64_t p = 3; p * p <= n; p += 2)
    {
        if ((n % p) != 0)
            continue;

        int exp = 0;
        while ((n % p) == 0)
        {
            n /= p;
            ++exp;
        }

        if ((p & 3ULL) == 1)          /* p ≡ 1 (mod 4) */
        {
            prod *= (uint64_t)(2 * exp + 1);
            if (prod >= 9)
                return true;
        }
    }

    /* remaining n is either 1 or a single prime > √k            */
    if (n > 1 && (n & 3ULL) == 1)   /* n ≡ 1 (mod 4) */
    {
        prod *= 3;   /* exponent is 1, so (2·1+1)=3                 */
        if (prod >= 9)
            return true;
    }

    return prod >= 9;
}

/* --------------------------------------------------------------------- */
/*  Dynamic array for cached terms                                        */
/* --------------------------------------------------------------------- */
static seq_t *seq     = NULL;
static size_t  seq_len = 0;
static size_t  seq_cap = 0;
static uint64_t next_candidate = 1;   /* next number to test           */

static void grow_seq(size_t new_len)
{
    if (new_len > seq_cap)
    {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        if (new_cap < new_len)
            new_cap = new_len;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) return;           /* on failure keep old pointer   */
        seq = tmp;
        seq_cap = new_cap;
    }
}

static void append_seq(seq_t val)
{
    grow_seq(seq_len + 1);
    seq[seq_len++] = val;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1, unused case */

    /* Produce terms until we reach the requested index */
    while (seq_len < (size_t)n)
    {
        if (satisfies(next_candidate))
            append_seq((seq_t)next_candidate);
        ++next_candidate;
    }
    return seq[n - 1];
}
