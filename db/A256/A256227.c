/*
 * A256227  Naught-y numbers (A011540) that after removing all zeros become
 *          zeroless primes (A038618).
 * https://oeis.org/A256227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 912 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A256227: Naught‑y numbers.
   An integer is accepted if it contains at least one decimal digit 0
   and the number obtained by removing all zeros is a prime that itself
   contains no zeros.  The function returns the n‑th such integer
   (offset 1).  Calls may occur in any order. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
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

/* Build the zero‑stripped version of n.
   The result has no zero digits and is returned in *out. */
static void strip_zero(uint64_t n, seq_t *out)
{
    uint8_t digits[20];        /* enough for any 64‑bit number   */
    size_t len = 0;

    while (n)
    {
        uint8_t d = n % 10;
        if (d != 0)
            digits[len++] = d;
        n /= 10;
    }

    seq_t y = 0;
    for (size_t i = len; i > 0; --i)
    {
        y = y * (seq_t)10 + (seq_t)digits[i - 1];
    }
    *out = y;
}

/* ----- static cache of found terms ----- */
static seq_t *seq = NULL;   /* array holding found terms          */
static size_t seq_len = 0;  /* number of terms currently stored   */
static size_t seq_cap = 0;  /* capacity of seq array              */

/* last integer examined during the incremental search */
static uint64_t cur = 1;    /* start at 1 so we find 10,20,...   */

/* Test whether a 64‑bit integer contains at least one decimal zero. */
static bool contains_zero(uint64_t n)
{
    while (n)
    {
        if ((n % 10) == 0)
            return true;
        n /= 10;
    }
    return false;
}

/* Ensure that the cached sequence contains at least n terms. */
static void ensure_n_terms(idx_t n)
{
    if (seq_len >= (size_t)n)
        return;

    if (seq_cap == 0)
    {
        seq_cap = 8;
        seq = mem_new(seq_t, seq_cap);
        if (!seq)
            exit(1);
    }

    while (seq_len < (size_t)n)
    {
        /* advance to the next integer that contains a zero */
        do
        {
            ++cur;
        } while (!contains_zero(cur));

        /* obtain the zero‑stripped number */
        seq_t stripped;
        strip_zero(cur, &stripped);

        /* accept the candidate if the stripped number is a prime
           that contains no zeros.  is_prime_big handles the required test. */
        if (is_prime_big(stripped))
        {
            if (seq_len == seq_cap)
            {
                seq_cap *= 2;
                seq_t *p = mem_resize(seq_t, seq, seq_cap);
                if (!p)
                    exit(1);
                seq = p;
            }
            seq[seq_len++] = (seq_t)cur;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* a(0) is undefined; return 0 for safety */

    ensure_n_terms(n);
    return seq[n - 1];
}
