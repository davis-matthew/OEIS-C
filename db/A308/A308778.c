/*
 * A308778  Central element(s) in the period of the continued fraction
 *          expansion of sqrt(n), or 0 if no such element exists, or -1 if n
 *          is a square.
 * https://oeis.org/A308778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = central element in the period of the continued fraction
 * expansion of sqrt(n). 
 * If n is a square, returns -1.
 * If the period has no central element (i.e. length 1), returns 0.
 *
 * Implementation: generates the periodic part of the continued fraction of
 * sqrt(n) and selects the central term(s).  The symmetric part has length
 * L−1 (where L is the period length).  If L==1, there is no central term.
 * For L≥2, the central index (0‑based) is L/2−1; the element there (and,
 * for odd L, the following element) is returned.
 */

#include <stdlib.h>
#include <stdint.h>

/* 64‑bit integer square root (floor).  Works for any non‑negative n. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 sq = (__int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Perfect square? */
    idx_t r = isqrt_u64(n);
    if ((__int128)r * r == n)
        return (seq_t)-1;   /* -1 for squares */

    /* Generate continued fraction of sqrt(n) until last term 2*r. */
    idx_t m = 0, d = 1, a_i = r;
    /* dynamic array to store the periodic terms a1 .. aL */
    size_t cap = 0, len = 0;
    idx_t *seq = NULL;

    for (;;) {
        /* next term */
        m = d * a_i - m;
        d = (n - m * m) / d;
        a_i = (r + m) / d;   /* floor division */

        /* store */
        if (len == cap) {
            size_t newcap = cap ? cap * 2 : 4;
            idx_t *tmp = (idx_t *)realloc(seq, newcap * sizeof(idx_t));
            if (!tmp) {
                free(seq);
                return (seq_t)0;  /* allocation failure → return 0 */
            }
            seq = tmp;
            cap = newcap;
        }
        seq[len++] = a_i;

        if (a_i == 2 * r)   /* period ends */
            break;
    }

    /* Period length L = len */
    idx_t L = len;
    if (L == 1) {          /* [r; 2*r] → no central element */
        free(seq);
        return (seq_t)0;
    }

    /* Central index (0‑based) : L/2 − 1  */
    idx_t central_idx = L / 2 - 1;
    seq_t result = (seq_t)seq[central_idx];

    free(seq);
    return result;
}
