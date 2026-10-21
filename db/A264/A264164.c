/*
 * A264164  3-smooth numbers whose number of divisors is not 3-smooth.
 * https://oeis.org/A264164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 249, u64 n <= 1154,
 *            u128 n <= 4936, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A264164 – 3‑smooth numbers whose number of divisors is not 3‑smooth.
    The function generates Hamming numbers 2^i⋅3^j in ascending order
    and keeps those whose divisor count (i+1)(j+1) contains a prime other
    than 2 or 3.  Results are memoised so the function works with
    arbitrary‑ordered calls. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *hams   = NULL;      /* all generated 2^i⋅3^j   */
    static size_t hams_len  = 0;
    static size_t hams_cap  = 0;
    static size_t i2 = 0, i3 = 0;

    static seq_t *list   = NULL;      /* answer list            */
    static size_t list_len  = 0;
    static size_t list_cap  = 0;

    /* initialise the generators on first call */
    if (!hams) {
        hams_cap = 16;
        hams = (seq_t*)malloc(hams_cap * sizeof(seq_t));
        hams[0] = (seq_t)1;           /* 2^0⋅3^0 */
        hams_len = 1;
        i2 = i3 = 0;
    }
    if (!list) {
        list_cap = 16;
        list = (seq_t*)malloc(list_cap * sizeof(seq_t));
        list_len = 0;
    }

    /* generate until the list contains at least n terms */
    while (list_len < (size_t)n) {
        seq_t n2 = hams[i2] * (seq_t)2;
        seq_t n3 = hams[i3] * (seq_t)3;
        seq_t nxt = (n2 < n3) ? n2 : n3;   /* next Hamming number */

        if (nxt == n2) ++i2;
        if (nxt == n3) ++i3;

        if (hams_len == hams_cap) {
            hams_cap *= 2;
            hams = (seq_t*)realloc(hams, hams_cap * sizeof(seq_t));
        }
        hams[hams_len++] = nxt;

        /* compute exponents of 2 and 3 in nxt */
        seq_t tmp = nxt;
        unsigned e2 = 0, e3 = 0;
        while (tmp % (seq_t)2 == 0) { tmp /= (seq_t)2; ++e2; }
        while (tmp % (seq_t)3 == 0) { tmp /= (seq_t)3; ++e3; }

        /* divisor count = (e2+1)(e3+1) */
        seq_t d = (seq_t)(e2 + 1) * (seq_t)(e3 + 1);

        /* test whether d is 3‑smooth */
        seq_t t = d;
        while (t % (seq_t)2 == 0) t /= (seq_t)2;
        while (t % (seq_t)3 == 0) t /= (seq_t)3;

        if (t != 1) {                 /* d has a prime > 3 → include */
            if (list_len == list_cap) {
                list_cap *= 2;
                list = (seq_t*)realloc(list, list_cap * sizeof(seq_t));
            }
            list[list_len++] = nxt;
        }
    }

    return list[(size_t)n - 1];
}
