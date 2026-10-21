/*
 * A276388  Write n in the P-base {1, 3, 4, 11, 15, 41, 56, 153, 209, ...}
 *          corresponding to (1+sqrt(3))/2 (see A002530).
 * https://oeis.org/A276388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the representation of n in the P‑base {1, 3, 4, 11, 15,
 *            41, 56, 153, 209, …} whose digits are 0,1,2. The output is the
 *            decimal concatenation of those digits
 * Range:     u8 n <= 10, u16 n <= 40, u32 n <= 779, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The weight sequence w satisfies w[0]=1, w[1]=3, w[2]=4, w[3]=11 and
   for i≥4:  w[i] = 4*w[i-2] – w[i-4].  This generates the values
   1,3,4,11,15,41,56,153,209,…  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset a(0)=0 directly. */
    if (n == 0) return 0;

    /* Static cache of weights.  wlen = number of stored weights. */
    static seq_t *w = NULL;
    static size_t wlen = 0, wcap = 0;

    /* Bootstrap initial weights on first call. */
    if (w == NULL) {
        wcap = 4;
        w = (seq_t*)malloc(wcap * sizeof(seq_t));
        w[0] = (seq_t)1;
        w[1] = (seq_t)3;
        w[2] = (seq_t)4;
        w[3] = (seq_t)11;
        wlen = 4;
    }

    /* Grow the weight table until the last weight exceeds n. */
    while (w[wlen-1] <= (seq_t)n) {
        seq_t next;
        /* recurrence   w[i] = 4*w[i-2] – w[i-4]   (for i≥4)  */
        next = w[wlen-2];
        next += next;   /* 2*w[i-2] */
        next += next;   /* 4*w[i-2] */
        next -= w[wlen-4];

        if (wlen == wcap) {
            wcap <<= 1;
            w = (seq_t*)realloc(w, wcap * sizeof(seq_t));
        }
        w[wlen++] = next;
    }

    /* Find the highest weight that does not exceed n. */
    size_t i = wlen - 1;
    while (i > 0 && w[i] > (seq_t)n) i--;

    seq_t rem = (seq_t)n;   /* remaining value to represent */
    seq_t res = 0;          /* resulting decimal number */

    /* Greedy construction of the representation. */
    for (int pos = (int)i; pos >= 0; pos--) {
        seq_t wi = w[(size_t)pos];
        seq_t d = rem / wi;          /* 0,1,2 … */
        if (d > (seq_t)2) d = (seq_t)2;
        rem -= d * wi;
        res = res * (seq_t)10 + d;
    }

    return res;
}
