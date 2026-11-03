/*
 * A331214  a(n) and at least one distinct anagram of a(n) that doesn't start
 *          with a leading 0 sum up to a palindrome.
 * https://oeis.org/A331214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n‑th integer m such that there exists a distinct
 *            anagram p of m (p does not start with 0) for which m + p is a
 *            palindrome
 * Range:     u8 n <= 41, u16 n <= 4092, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9758 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/*------------------------------------------------------------
  Helper: compare two byte arrays for equality.
  ------------------------------------------------------------*/
static bool same_as_orig(const unsigned char *perm, const unsigned char *orig,
                         size_t len)
{
    for (size_t i = 0; i < len; ++i)
        if (perm[i] != orig[i]) return false;
    return true;
}

/*------------------------------------------------------------
  next_permutation on an array of unsigned char.
  Returns true if a new permutation was produced,
  false if it was the last one.
  ------------------------------------------------------------*/
static bool next_permutation(unsigned char *a, size_t n)
{
    if (n < 2) return false;
    int i = (int)n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) i--;
    if (i < 0) return false;
    int j = (int)n - 1;
    while (a[j] <= a[i]) j--;
    unsigned char t = a[i]; a[i] = a[j]; a[j] = t;
    int l = i + 1, r = (int)n - 1;
    while (l < r) {
        t = a[l]; a[l] = a[r]; a[r] = t;
        l++; r--;
    }
    return true;
}

/*------------------------------------------------------------
  Check whether a decimal representation of x is a palindrome.
  ------------------------------------------------------------*/
static bool is_palindrome(seq_t x)
{
    static unsigned char *buf = NULL;
    static size_t buf_cap = 0;
    size_t len = 0;
    seq_t t = x;

    while (t > 0) {
        if (len == buf_cap) {
            buf_cap = buf_cap ? buf_cap * 2 : 16;
            buf = (unsigned char *)realloc(buf, buf_cap);
        }
        buf[len++] = (unsigned char)(t % 10);
        t /= 10;
    }
    for (size_t i = 0; i < len / 2; ++i)
        if (buf[i] != buf[len - 1 - i]) return false;
    return true;
}

/*------------------------------------------------------------
  Test whether a given number m satisfies the definition of the
  sequence.
  ------------------------------------------------------------*/
static bool is_good(seq_t m)
{
    /* Extract digits of m into a LSB-first array (64 is room for any m
       here; this used to call malloc on every pass, leaking each block, and
       compare its result with len as a "placeholder"). */
    unsigned char *digits = (unsigned char *)malloc(64);
    if (!digits) abort();
    size_t len = 0;
    for (seq_t t = m; t > 0 && len < 64; t /= 10)
        digits[len++] = (unsigned char)(t % 10);
    if (len == 0) digits[len++] = 0;

    /* Build MSB-first original array. */
    unsigned char *orig = (unsigned char *)malloc(len);
    for (size_t i = 0; i < len; ++i)
        orig[i] = digits[len - 1 - i];

    /* Build permutation array by copying orig. */
    unsigned char *perm = (unsigned char *)malloc(len);
    memcpy(perm, orig, len);

    /* Sort perm ascending (simple insertion sort). */
    for (size_t i = 1; i < len; ++i) {
        unsigned char key = perm[i];
        int j = (int)i - 1;
        while (j >= 0 && perm[j] > key) {
            perm[j + 1] = perm[j];
            --j;
        }
        perm[j + 1] = key;
    }

    /* Enumerate all permutations. */
    do {
        if (perm[0] == 0) continue;          /* leading zero not allowed */
        if (same_as_orig(perm, orig, len)) continue; /* must be distinct */
        /* Build integer p from permutation. */
        seq_t p = 0;
        for (size_t i = 0; i < len; ++i)
            p = p * 10 + perm[i];
        if (is_palindrome(m + p)) {
            free(digits);
            free(orig);
            free(perm);
            return true;
        }
    } while (next_permutation(perm, len));

    free(digits);
    free(orig);
    free(perm);
    return false;
}

/*------------------------------------------------------------
  Sequence storage: dynamic array of found terms.
  ------------------------------------------------------------*/
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static seq_t next_candidate = 12;   /* first number to examine */

/*------------------------------------------------------------
  Public sequence function.
  ------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for zero index, harmless */

    /* Ensure we have at least n terms cached. */
    while (terms_len < (size_t)n) {
        /* Find the next qualifying number. */
        while (!is_good(next_candidate))
            ++next_candidate;

        /* Store it, allocating more space if necessary. */
        if (terms_len >= terms_cap) {
            terms_cap = terms_cap ? terms_cap * 2 : 16;
            terms = mem_resize(seq_t, terms, terms_cap);
        }
        terms[terms_len++] = next_candidate;
        ++next_candidate;         /* move on to the next integer */
    }

    return terms[n - 1];
}
