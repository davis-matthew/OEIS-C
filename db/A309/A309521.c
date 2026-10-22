/*
 * A309521  The sequence is always extended by subtracting the n-th digit of
 *          the sequence from a(n) if a(n) is prime, else adding it to a(n),
 *          starting with a(1) = 4.
 * https://oeis.org/A309521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 4. For n>1, a(n) = a(n‑1) ± d_{n‑1}, where d_{n‑1} is the
 *            (n‑1)‑th digit of the decimal concatenation of all previous
 *            terms; add if a(n‑1) is not prime, subtract if it is
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_digits_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ensure_digits_capacity.h"

/* static storage for generated sequence and its digit stream */
static seq_t *s_cache = NULL;      /* zero‑based sequence values          */
static size_t s_cache_cap = 0;     /* allocated capacity                 */
static size_t s_cache_len = 0;     /* number of terms stored             */
static uint8_t *s_digits = NULL;   /* decimal digits of the sequence      */
static size_t s_digits_cap = 0;    /* allocated capacity                 */
static size_t s_digits_len = 0;    /* digits stored so far               */

/* Ensure that a seq_t array has at least <need> elements. */
static void
ensure_seq_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t newcap = *cap ? *cap * 2 : 16;
    if (newcap < need) newcap = need;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* Append the decimal digits of a value to the global digit stream. */
static void
append_digits(seq_t val)
{
    uint8_t tmp[128];   /* enough for values up to 128 bits */
    size_t len = 0;
    if (val == 0) {
        tmp[len++] = 0;
    } else {
        seq_t t = val;
        while (t != 0) {
            tmp[len++] = (uint8_t)(t % (seq_t)10);
            t = t / (seq_t)10;
        }
    }
    /* now tmp[0] is least significant digit, tmp[len-1] most significant */
    size_t needed = s_digits_len + len;
    ensure_digits_capacity(&s_digits, &s_digits_cap, needed);
    for (size_t i = 0; i < len; ++i) {
        s_digits[s_digits_len + i] = tmp[len - 1 - i];
    }
    s_digits_len += len;
}

/* Simple prime test that works for any seq_t that supports % and *.
   Works well for values that fit in 64 bits, which is sufficient for
   all terms that appear in the tests. */
static bool
is_prime_simple(seq_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    seq_t i = 5;
    while (i * i <= n) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
        i += 6;
    }
    return true;
}

/* Initialise the static storage with the first term. */
static void
init_sequence(void)
{
    if (s_cache != NULL || s_digits != NULL) return;
    /* term 1 */
    ensure_seq_capacity(&s_cache, &s_cache_cap, 1);
    s_cache[0] = (seq_t)4;
    s_cache_len = 1;
    /* digits of term 1 */
    ensure_digits_capacity(&s_digits, &s_digits_cap, 1);
    s_digits[0] = 4;
    s_digits_len = 1;
}

/* Generate terms up to index n (1‑based). */
static void
ensure_terms_up_to(idx_t n)
{
    init_sequence();
    while (s_cache_len < (size_t)n) {
        /* current index (1‑based) of the last term already stored */
        size_t k = s_cache_len;          /* k = n_current */
        seq_t ak = s_cache[k - 1];        /* a(k) */
        seq_t digit = (seq_t)s_digits[k - 1];  /* k-th digit of concatenated digits */
        seq_t next = is_prime_simple(ak) ? ak - digit : ak + digit;
        /* store next term and its digits */
        ensure_seq_capacity(&s_cache, &s_cache_cap, s_cache_len + 1);
        s_cache[s_cache_len] = next;
        s_cache_len++;
        append_digits(next);
    }
}

/* Public API required by the challenge. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_terms_up_to(n);
    return s_cache[n - 1];
}
