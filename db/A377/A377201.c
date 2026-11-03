/*
 * A377201  Numbers k such that d(k)^k == d(k) (mod k), where d(k) =
 *          A000005(k) is the number of divisors of k.
 * https://oeis.org/A377201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 6777, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 8666 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A377201
 * Numbers k such that d(k)^k == d(k) (mod k),
 * where d(k) = A000005(k) is the number of divisors of k.
 * The routine lazily generates the sequence up to the requested index
 * by testing successive integers.  It keeps a dynamic table of the
 * found terms in a uint64_t array and returns the n‑th term cast to
 * seq_t.  The calculation uses divisor_count_u64 and mod_pow from
 * the util library.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Count divisors of a 64‑bit integer */
static uint64_t divisor_count_u64(uint64_t n)
{
    uint64_t tau = 1;
    uint64_t exp;

    /* factor 2 */
    exp = 0;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        ++exp;
    }
    if (exp) tau *= (exp + 1);

    /* factor odd primes */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }

    /* remaining prime factor */
    if (n > 1) tau *= 2;

    return tau;
}

/* modular exponentiation using 128‑bit intermediate.
   Works for any seq_t. */
static seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t res = 1 % mod;
    base   %= mod;
    while (exp) {
        if (exp & 1)
            res = (unsigned __int128)res * base % mod;
        exp >>= 1;
        if (exp)
            base = (unsigned __int128)base * base % mod;
    }
    return res;
}

static uint64_t *terms = NULL;   /* table of found terms */
static size_t   terms_len = 0;   /* number of stored terms   */
static size_t   terms_cap = 0;   /* allocated capacity       */
static idx_t    last_checked = 0;/* largest k that has been examined */

seq_t a(idx_t n)
{
    /* grow the table until we have at least n terms */
    while (terms_len < n)
    {
        ++last_checked;                          /* next integer to test */

        /* number of divisors of the current integer */
        uint64_t d = divisor_count_u64(last_checked);

        /* compute d^last_checked (mod last_checked)     */
        seq_t powmod = mod_pow((seq_t)d, last_checked, (seq_t)last_checked);

        /* d modulo last_checked */
        seq_t dmod = (seq_t)(d % last_checked);

        if (powmod == dmod)                      /* condition satisfied */
        {
            if (terms_len == terms_cap)          /* grow the array */
            {
                terms_cap = (terms_cap == 0) ? 32 : terms_cap * 2;
                terms = (uint64_t *)realloc(terms, terms_cap * sizeof(uint64_t));
            }
            terms[terms_len++] = last_checked;   /* store the term */
        }
    }

    /* return the requested term, converted to seq_t */
    return (seq_t)terms[n - 1];
}
