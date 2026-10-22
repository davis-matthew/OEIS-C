/*
 * A393588  Expansion of 1/((1-z)*(1-2*z)*(1-3*z))^3.
 * https://oeis.org/A393588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A393588
   Formula: a(n) = Sum_{i+j+k=n} binom(i+2,2) * binom(j+2,2) * 2^j * binom(k+2,2) * 3^k
   Recurrence (order 9):
     a(0)=1,
     a(1)=18,
     a(2)=183,
     a(3)=1386,
     a(4)=8718,
     a(5)=48204,
     a(6)=242434,
     a(7)=1134108,
     a(8)=5011791,
     for n≥9
        a(n)=18*a(n-1)-141*a(n-2)+630*a(n-3)-1767*a(n-4)+3222*a(n-5)
             -3815*a(n-6)+2826*a(n-7)-1188*a(n-8)+216*a(n-9) */
#include <stdlib.h>

seq_t a(idx_t n) {
    static idx_t max_n = 8;          /* highest index computed so far */
    static size_t cap = 0;           /* allocated capacity */
    static seq_t *buf = NULL;        /* array of computed terms */

    /* ensure we have space at least up to n (or to 8 for recurrence) */
    idx_t required = n;
    if (required < 8) required = 8;  /* we need terms up to 8 for recurrence */
    size_t new_cap = (size_t)(required + 1);

    if (cap < new_cap) {
        if (cap == 0) {
            /* first allocation – reserve at least 9 elements and initialise them */
            cap = new_cap;
            buf = (seq_t *)malloc(cap * sizeof(seq_t));
            buf[0] = 1;
            buf[1] = 18;
            buf[2] = 183;
            buf[3] = 1386;
            buf[4] = 8718;
            buf[5] = 48204;
            buf[6] = 242434;
            buf[7] = 1134108;
            buf[8] = 5011791;
            max_n = 8;
        } else {
            /* grow the buffer */
            buf = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
            cap = new_cap;
        }
    }

    /* compute any missing terms up to requested index n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val = (seq_t)18  * buf[i - 1];
        val -= (seq_t)141 * buf[i - 2];
        val += (seq_t)630  * buf[i - 3];
        val -= (seq_t)1767 * buf[i - 4];
        val += (seq_t)3222 * buf[i - 5];
        val -= (seq_t)3815 * buf[i - 6];
        val += (seq_t)2826 * buf[i - 7];
        val -= (seq_t)1188 * buf[i - 8];
        val += (seq_t)216  * buf[i - 9];
        buf[i] = val;
    }
    if (n > max_n) max_n = n;

    return buf[n];
}
