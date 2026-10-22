/*
 * A343714  Palindromic primes of the form p//q//reverse(p), where p is a
 *          prime (not necessarily palindromic) and q, of course, is a
 *          palindromic prime.
 * https://oeis.org/A343714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 13, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   is_palindrome, is_prime_big, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A343714: palindromic primes of the form p//q//reverse(p)
   where p is a prime, q is a palindromic prime.  The function
   generates palindromic primes in ascending order, tests each for a
   valid decomposition, stores the found terms, and returns the
   n‑th term.  The algorithm is O(n · m) where m is the average
   number of digits in the candidates (well below the required limit
   for n ≤ 10000). */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* Utility prototypes (provided by the build). */
#include "../utils/seq_from_str.h"
#include "../utils/is_prime_big.h"
#include "../utils/is_palindrome.h"

/* Convert an unsigned 64‑bit integer to a decimal string.
   The caller must provide a buffer of at least 32 characters. */
static void uint64_to_str(uint64_t n, char *buf, size_t *len)
{
    if (n == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char rev[32];
    size_t i = 0;
    while (n > 0) {
        rev[i++] = (char)('0' + (n % 10));
        n /= 10;
    }
    *len = i;
    for (size_t j = 0; j < i; ++j)
        buf[j] = rev[i - 1 - j];
}

/* Helper to append a new term to the dynamic array of terms. */
static void append_term(seq_t **terms, size_t *cnt, size_t *cap, seq_t value)
{
    if (*cnt == *cap) {
        size_t newcap = (*cap == 0) ? 8 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*terms, newcap * sizeof(seq_t));
        if (!tmp) return; /* Allocation failed – caller will eventually stop. */
        *terms = tmp;
        *cap = newcap;
    }
    (*terms)[*cnt] = value;
    (*cnt)++;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Cache of found terms. */
    static seq_t *terms = NULL;
    static size_t term_count = 0;
    static size_t capacity   = 0;
    static uint64_t next_half = 1;   /* next integer to use as half of a palindrome */

    while (term_count < n) {
        /* Build decimal string of the current half. */
        char half_str[32];
        size_t half_len;
        uint64_to_str(next_half, half_str, &half_len);

        /* Build the odd‑length palindrome: half + reverse(half without last digit). */
        char pal_str[512];
        size_t pal_len = 0;
        for (size_t i = 0; i < half_len; ++i)
            pal_str[pal_len++] = half_str[i];
        for (int i = (int)half_len - 2; i >= 0; --i)
            pal_str[pal_len++] = half_str[i];
        pal_str[pal_len] = '\0';

        /* Convert palindrome string to seq_t. */
        seq_t pal = seq_from_str(pal_str);

        /* Check primality of the palindrome. */
        if (is_prime_big(pal)) {
            /* Try all possible splits: p (length s), q (middle), reverse(p). */
            bool found = false;
            for (size_t s = 1; s * 2 < pal_len; ++s) {
                size_t mid_len = pal_len - 2 * s;
                if (mid_len == 0) break;          /* middle must be at least 1 digit */

                /* Prefix p */
                char p_str[256];
                memcpy(p_str, pal_str, s);
                p_str[s] = '\0';
                seq_t p = seq_from_str(p_str);
                if (!is_prime_big(p)) continue;

                /* Middle q */
                char q_str[256];
                memcpy(q_str, pal_str + s, mid_len);
                q_str[mid_len] = '\0';
                seq_t q = seq_from_str(q_str);
                if (!is_prime_big(q)) continue;
                if (!is_palindrome(q, 10)) continue; /* q must be palindromic prime */

                /* Both conditions satisfied ⇒ palindrome is a term. */
                found = true;
                break;
            }
            if (found) {
                append_term(&terms, &term_count, &capacity, pal);
            }
        }
        ++next_half;
    }
    return terms[n - 1];
}
