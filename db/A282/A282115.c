/*
 * A282115  Numbers m with k digits in base b (MSD(m)=d_k, LSD(m)=d_1) such
 *          that, for one of their digits in position d_k < j < d_1,
 *          Sum_{i=j+1..k} (i-j)*d_i = Sum_{i=1..j-1} (j-i)*d_i. Case b = 10.
 * https://oeis.org/A282115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 3577, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9375 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A282115: the nth integer that has a fulcrum digit j  
 * (1 < j < k) such that sum_{i=j+1..k} (i-j)*d_i == sum_{i=1..j-1} (j-i)*d_i  
 * in base 10, where the digits are d_k … d_1 (MSD … LSD).  
 * The function generates terms on demand, caches them, and returns the  
 * nth term (1‑based).  There is no closed‑form formula, so a search +   
 * memoisation approach is used. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static bool fulcrum_digit(seq_t x)
{
    /* Extract decimal digits into an array (LSD first). */
    unsigned char lsd_digits[256];
    size_t len = 0;
    seq_t t = x;

    if (t == (seq_t)0) {
        lsd_digits[0] = 0;
        len = 1;
    } else {
        while (t != (seq_t)0) {
            lsd_digits[len++] = (unsigned char)(t % (seq_t)10);
            t /= (seq_t)10;
        }
    }

    /* Numbers with fewer than 3 digits cannot have an internal digit j. */
    if (len < 3) return false;

    /* Build MSD‑first array for easier index handling. */
    unsigned char msd[256];
    for (size_t i = 0; i < len; ++i) msd[i] = lsd_digits[len - 1 - i];

    /* Quick check: odd‑length palindromes satisfy the condition. */
    if ((len & 1u) != 0) {
        bool pal = true;
        for (size_t i = 0; i < len / 2; ++i) {
            if (msd[i] != msd[len - 1 - i]) {
                pal = false; break;
            }
        }
        if (pal) return true;
    }

    /* Test every internal position j (1‑based from left). */
    for (size_t pos = 1; pos + 1 < len; ++pos) {
        seq_t left  = 0;
        seq_t right = 0;
        /* Sum over digits to the right of pos (higher indices). */
        for (size_t idx = pos + 1; idx < len; ++idx)
            left += (seq_t)(idx - pos) * (seq_t)msd[idx];
        /* Sum over digits to the left of pos. */
        for (size_t idx = 0; idx < pos; ++idx)
            right += (seq_t)(pos - idx) * (seq_t)msd[idx];
        if (left == right) return true;
    }
    return false;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 1;   /* candidate to test next */

/* Return a(n) for the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for 0, but return 0 safely */

    while (cache_len < (size_t)n) {
        seq_t cand = (seq_t)last_checked;
        if (fulcrum_digit(cand))
            append_candidate(&cache, &cache_len, &cache_cap, cand);
        ++last_checked;
    }
    return cache[(size_t)(n - 1)];
}
