/*
 * A262040  Nearest palindrome to n; in case of a tie choose the smaller
 *          palindrome.
 * https://oeis.org/A262040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   is_palindrome, seq_from_str, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: nearest palindrome to n, rounding down on a tie.
   The algorithm mirrors the first half of the decimal representation
   of n to construct a lower and an upper candidate and chooses the
   closer one (preferring the smaller when equal). */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_to_str.h"
#include "../utils/seq_from_str.h"
#include "../utils/is_palindrome.h"

static void build_pal(const char *prefix, int p_len, int total_len, char *out)
{
    for (int i = 0; i < total_len; ++i) {
        if (i < p_len)
            out[i] = prefix[i];
        else
            out[i] = prefix[total_len - 1 - i];
    }
    out[total_len] = '\0';
}

static seq_t small_all_nines(int len)
{
    if (len <= 0) return (seq_t)0;
    char buf[len];
    for (int i = 0; i < len; ++i) buf[i] = '9';
    buf[len] = '\0';
    return seq_from_str(buf);
}

static seq_t one_big_palin(int len)
{
    char buf[len + 2];
    buf[0] = '1';
    for (int i = 1; i < len; ++i) buf[i] = '0';
    buf[len] = '1';
    buf[len + 1] = '\0';
    return seq_from_str(buf);
}

seq_t a(idx_t n)
{
    /* n == 0 */
    if (n == (idx_t)0) return (seq_t)0;

    seq_t x = (seq_t)n;
    if (is_palindrome(x, 10)) return x;  /* n is already a palindrome */

    /* convert n to decimal string */
    char buf[1024];
    size_t blen = 0;
    seq_to_str(x, buf, &blen);
    int d = (int)blen;          /* number of decimal digits */

    int half = (d + 1) / 2;     /* length of the prefix defining the palindrome */

    /* prefix string */
    char prefix[512];
    memcpy(prefix, buf, half);
    prefix[half] = '\0';

    /* construct base palindrome by mirroring */
    char pal_buf[1024];
    build_pal(prefix, half, d, pal_buf);
    seq_t cand = seq_from_str(pal_buf);

    seq_t down, up;

    /* --- Downward palindrome --- */
    if (cand <= x) {
        down = cand;
    } else {
        /* decrement the prefix */
        seq_t p = seq_from_str(prefix);
        p -= (seq_t)1;
        size_t plen = 0;
        /* convert back to string */
        char pbuf[512];
        seq_to_str(p, pbuf, &plen);
        /* if decrement caused loss of a digit or leading zero, use all-9s of length d-1 */
        if (plen < (size_t)half || pbuf[0] == '0') {
            down = small_all_nines(d - 1);
        } else {
            /* pad with leading zeros if necessary to maintain half length */
            char pref2[512];
            int pad = half - (int)plen;
            for (int i = 0; i < pad; ++i) pref2[i] = '0';
            memcpy(pref2 + pad, pbuf, (int)plen);
            pref2[half] = '\0';
            build_pal(pref2, half, d, pal_buf);
            down = seq_from_str(pal_buf);
        }
    }

    /* --- Upward palindrome --- */
    if (cand >= x) {
        up = cand;
    } else {
        /* increment the prefix */
        seq_t p = seq_from_str(prefix);
        p += (seq_t)1;
        size_t plen = 0;
        char pbuf[512];
        seq_to_str(p, pbuf, &plen);
        if ((int)plen > half) {   /* overflow to one more digit */
            up = one_big_palin(d);
        } else {
            /* pad if needed */
            char pref2[512];
            int pad = half - (int)plen;
            for (int i = 0; i < pad; ++i) pref2[i] = '0';
            memcpy(pref2 + pad, pbuf, (int)plen);
            pref2[half] = '\0';
            build_pal(pref2, half, d, pal_buf);
            up = seq_from_str(pal_buf);
        }
    }

    /* decide between down and up based on distance */
    seq_t diff_down = (x >= down) ? x - down : down - x;
    seq_t diff_up   = (x <= up)   ? up   - x : x - up;

    if (diff_down <= diff_up)
        return down;
    else
        return up;
}
