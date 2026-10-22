/*
 * A317387  Square array T(n, k) read by antidiagonals upwards, n > 0 and k >
 *          0: T(n, k) is the least positive multiple of n that contains k as
 *          a substring in its decimal representation.
 * https://oeis.org/A317387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A317387:  a(n) = T(i,j) where (i,j) are the indices of the n‑th
   entry read by antidiagonals.  T(i,j) is the least positive multiple of
   i that contains j as a decimal substring. */

#include <stddef.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* Helpers for converting numbers to decimal strings (most significant
   digit first).  The implementation works for builtin unsigned types
   as well as for arbitrary‑precision integer classes that support
   modulo and division by unsigned 10 and comparison with 0.  */

/* Produce the decimal representation of v in buf[0..len-1] (MSB first).  */
static void seq_to_string(seq_t v, char *buf, size_t *len)
{
    size_t idx = 0;
    if (v == 0) {
        buf[idx++] = '0';
    } else {
        seq_t tmp = v;
        /* Store digits in reverse order */
        while (tmp != 0) {
            unsigned int d = (unsigned int)(tmp % 10);
            buf[idx++] = '0' + d;
            tmp = tmp / 10;
        }
        /* Reverse into MSB first order */
        for (size_t i = 0; i < idx / 2; ++i) {
            char t = buf[i];
            buf[i] = buf[idx - 1 - i];
            buf[idx - 1 - i] = t;
        }
    }
    *len = idx;
}

/* Check whether buf[0..mlen-1] contains pat[0..plen-1] as a contiguous
   substring.  */
static int contains_substring(const char *buf, size_t mlen,
                              const char *pat, size_t plen)
{
    if (plen == 0 || plen > mlen) return 0;
    for (size_t i = 0; i + plen <= mlen; ++i) {
        size_t j;
        for (j = 0; j < plen; ++j) {
            if (buf[i + j] != pat[j]) break;
        }
        if (j == plen) return 1;
    }
    return 0;
}

/* Compute the least positive multiple of n that contains the decimal
   representation of k.  */
static seq_t T(idx_t n, idx_t k)
{
    if (n == 1) return (seq_t)k;
    if (n == k) return (seq_t)n;

    /* Convert k to a string once.  */
    char pat[128];
    size_t pat_len;
    seq_to_string((seq_t)k, pat, &pat_len);

    seq_t m = (seq_t)n;
    while (1) {
        char buf[256];
        size_t mlen;
        seq_to_string(m, buf, &mlen);
        if (contains_substring(buf, mlen, pat, pat_len))
            return m;
        m = m + (seq_t)n;
    }
}

/* Find the (i,j) coordinates of the position pos in the
   anti‑diagonal reading (n>0,k>0).  Only integer arithmetic is used. */
static void antidiagonal_coords(idx_t pos, idx_t *i, idx_t *j)
{
    /* Find smallest d such that d*(d+1)/2 >= pos. */
    idx_t lo = 0, hi = 1;
    while ((((idx_t)(__int128)hi * (hi + 1)) / 2) < pos)
        hi <<= 1;                  /* double hi */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t sum = ((idx_t)(__int128)mid * (mid + 1)) / 2;
        if (sum >= pos) hi = mid;
        else            lo = mid + 1;
    }
    idx_t d = hi;   /* diagonal number, 1‑based */

    idx_t prev_sum = ((idx_t)(__int128)(d - 1) * d) / 2;
    idx_t offset = pos - prev_sum;        /* 1‑based offset on this diagonal */
    idx_t idx0   = offset - 1;            /* 0‑based offset */

    *i = d - idx0;            /* n‑coordinate */
    *j = idx0 + 1;            /* k‑coordinate */
}

/* --------------------------------------------------------------------- */
/* Public function to be called by the OEIS testing harness.  */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return T(i, j);
}
