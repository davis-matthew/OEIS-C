/*
 * A248754  Palindromes p=A002113(n) whose index n is also a palindrome and in
 *          addition a substring of p (strings in base 10).
 * https://oeis.org/A248754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A248754: palindromic numbers p = A002113(n) whose index n
 * is also a palindrome and for which n appears as a decimal substring of
 * p.  The function searches incrementally over the palindromic indices
 * until the required term is found.
 *   a(1) = 11
 *   a(2) = 1111
 *   ...
 * No hard‑coded table is used. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- helpers for palindrome indices ---------- */
static bool is_palindrome_idx(idx_t v)
{
    idx_t rev = 0, t = v;
    if (v == 0) return true;
    while (t > 0) {
        rev = rev * 10 + t % 10;
        t /= 10;
    }
    return rev == v;
}

static idx_t pow10_idx(idx_t e)
{
    idx_t r = 1;
    while (e--) r *= 10;
    return r;
}

/* ---------- helper: build the nth palindrome (1‑based) ---------- */
static seq_t nth_palindrome(idx_t pos)
{
    /* pos is 1‑based; internally use 0‑based k */
    idx_t k = pos - 1;            /* zero based index in A002113 */
    if (k < 10)                    /* single‑digit palindromes including 0 */
        return (seq_t)k;

    k -= 10;                       /* skip the 10 single‑digit ones */
    size_t len = 1;                /* current length of palindromes considered */
    while (1) {
        ++len;
        idx_t cnt = 9 * pow10_idx((len - 1) / 2);
        if (k < cnt) break;
        k -= cnt;
    }

    size_t half = (len + 1) / 2;
    idx_t start = pow10_idx(half - 1);          /* first half lower bound */
    idx_t first_half = start + k;               /* first half value */

    /* build palindrome from first_half */
    seq_t pal = (seq_t)first_half;              /* upper half as seq_t */
    idx_t x = first_half;
    if (len & 1) x /= 10;                      /* skip middle digit if odd length */

    while (x > 0) {
        pal = pal * 10 + (seq_t)(x % 10);
        x /= 10;
    }
    return pal;
}

/* ---------- helpers to obtain digit arrays ---------- */
static void seq_digits(seq_t x, uint8_t *buf, size_t *len)
{
    size_t i = 0;
    if (x == 0) { buf[0] = 0; *len = 1; return; }
    while (x > 0) {
        buf[i++] = (uint8_t)(x % 10);
        x /= 10;
    }
    /* reverse to get most‑significant digit first */
    for (size_t l = 0, r = i - 1; l < r; ++l, --r) {
        uint8_t tmp = buf[l];
        buf[l] = buf[r];
        buf[r] = tmp;
    }
    *len = i;
}

static void idx_digits(idx_t x, uint8_t *buf, size_t *len)
{
    size_t i = 0;
    if (x == 0) { buf[0] = 0; *len = 1; return; }
    while (x > 0) {
        buf[i++] = (uint8_t)(x % 10);
        x /= 10;
    }
    for (size_t l = 0, r = i - 1; l < r; ++l, --r) {
        uint8_t tmp = buf[l];
        buf[l] = buf[r];
        buf[r] = tmp;
    }
    *len = i;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, guard anyway */

    idx_t found = 0;        /* how many terms found so far */
    idx_t candidate = 1;    /* current palindrome index to test (first >0) */

    while (found < n) {
        /* skip indices that are not decimal palindromes */
        if (!is_palindrome_idx(candidate)) {
            ++candidate;
            continue;
        }

        /* candidate is a palindrome index; compute its palindrome value */
        seq_t p = nth_palindrome(candidate);

        /* obtain digit arrays of p and candidate */
        uint8_t bufP[1024], bufC[1024];
        size_t lenP, lenC;
        seq_digits(p, bufP, &lenP);
        idx_digits(candidate, bufC, &lenC);

        /* check if candidate digits are a contiguous substring of p digits */
        bool match = false;
        if (lenC <= lenP) {
            for (size_t i = 0; i <= lenP - lenC; ++i) {
                bool ok = true;
                for (size_t j = 0; j < lenC; ++j) {
                    if (bufP[i + j] != bufC[j]) { ok = false; break; }
                }
                if (ok) { match = true; break; }
            }
        }

        if (match) {
            ++found;
            if (found == n) return p;
        }

        ++candidate;
    }

    return 0;  /* should never reach here for valid n */
}
