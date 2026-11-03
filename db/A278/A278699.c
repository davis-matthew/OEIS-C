/*
 * A278699  Primes p such that every suffix of the base-4 representation of p
 *          is a prime.
 * https://oeis.org/A278699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Brute‑force search for primes p such that every suffix
   of the base‑4 representation of p is prime.  The function
   a(n) returns the n‑th element (1‑based) of OEIS sequence A278699. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Check whether each suffix of the base‑4 representation of p is prime. */
static bool check_suffix_prime(seq_t p)
{
    if (p == (seq_t)2)             /* 2 is a special term */
        return true;

    seq_t temp    = p;
    seq_t suffix  = 0;
    seq_t pow4    = 1;

    /* Iterate over base‑4 digits from least significant to most. */
    while (temp > 0) {
        seq_t digit = temp & 3;            /* temp % 4 */
        suffix += digit * pow4;            /* build current suffix */
        if (!is_prime_big(suffix))          /* suffix must be prime */
            return false;
        pow4 *= 4;                          /* next power of 4  */
        temp >>= 2;                         /* temp /= 4 */
    }
    return true;                              /* all suffixes OK */
}

/* ------------------------------------------------------------------ */
/* Cache of already found terms.  The cache is expanded lazily. */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;   /* number of terms stored */
static size_t seq_cap  = 0;   /* allocated capacity */
static seq_t  last_candidate;/* next candidate to test (always %4==3) */

/* ------------------------------------------------------------------ */
/* Ensure the cache contains at least 'needed' terms. */
static void ensure_seq_size(idx_t needed)
{
    /* First call: allocate and seed cache. */
    if (seq_cache == NULL) {
        seq_cap = 16;
        seq_cache = mem_new(seq_t, seq_cap);
        seq_len   = 0;
        seq_cache[0] = (seq_t)2;   /* first term of the sequence */
        seq_len      = 1;
        last_candidate = (seq_t)3; /* next prime candidate (4n+3) */
    }

    /* Generate new terms until the cache is large enough. */
    while (seq_len < (size_t)needed) {
        for (seq_t cand = last_candidate; ; cand += 4) {
            /* Only consider primes. */
            if (!is_prime_big(cand))
                continue;

            /* Check suffix property. */
            if (check_suffix_prime(cand)) {
                /* Grow cache if needed. */
                if (seq_len == seq_cap) {
                    seq_cap <<= 1;
                    seq_cache = mem_resize(seq_t, seq_cache, seq_cap);
                }
                seq_cache[seq_len++] = cand;
                last_candidate = cand + 4; /* next candidate */
                break;                     /* back to outer loop */
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;       /* undefined for n<1 */

    if (seq_cache == NULL)
        ensure_seq_size((idx_t)1);

    if ((size_t)n > seq_len)
        ensure_seq_size(n);

    return seq_cache[(size_t)n - 1];
}
