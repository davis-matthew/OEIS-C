/*
 * A367066  a(n) = ((Sum_{i=1..n} A367065(i))-2)/(n+2).
 * https://oeis.org/A367066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((Sum_{i=1..n} b(i)) - 2) / (n + 2) with b(1) = 1 and for
 *            i > 1, b(i) is the smallest positive integer not yet used such
 *            that Sum_{j=1..i} b(j) ≡ i (mod i+2). (This is the
 *            Avdispahić-Zejnulahi means sequence AZM(2).) The function
 *            recomputes the sequence from scratch for each call, ensuring...
 * Range:     u8 n <= 19, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static unsigned char *used = NULL;          /* bitmap of used integers */
static size_t used_cap = 0;                 /* capacity of used bitmap */

/* Reallocate the bitmap to at least 'cap' elements and clear it. */
static void
reset_used(size_t cap)
{
    if (used) {
        free(used);
        used = NULL;
    }
    used = (unsigned char *)calloc((size_t)cap, (size_t)1);
    if (used) {
        used_cap = cap;
    } else {
        /* allocation failed – leave used_cap 0, the algorithm will
           try again on the next call (unlikely in typical tests). */
        used_cap = 0;
    }
}

/* Ensure that bitmap can hold indices up to 'idx' (inclusive) */
static void
ensure_used_capacity(idx_t idx)
{
    if ((size_t)idx < used_cap) return;
    /* double the capacity until large enough */
    size_t newcap = used_cap ? used_cap : 16;
    while ((size_t)idx >= newcap) newcap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, newcap);
    if (tmp) {
        /* zero newly allocated part */
        memset(tmp + used_cap, 0, newcap - used_cap);
        used = tmp;
        used_cap = newcap;
    } else {
        /* allocation failed – treat as failure for this call */
    }
}

/* Public API: return a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;              /* undefined for n<1 */

    /* Estimate required bitmap size.  The values b(i) grow roughly
       linearly; 2*n is sufficient margin for n up to 10000. */
    size_t cap = (size_t)(n * 2 + 64);
    reset_used(cap);

    seq_t sum_b = 0;
    for (idx_t cur = 1; cur <= n; ++cur) {
        idx_t modv = cur + 2;                         /* modulus is i+2 */
        idx_t rem  = (idx_t)(sum_b % (seq_t)modv);
        idx_t delta = (2 + modv - rem) % modv;
        idx_t residue = (delta == 0) ? modv : delta; /* first candidate */

        idx_t val = residue;
        for (;;) {
            ensure_used_capacity(val);
            if (!used[(size_t)val]) break;
            val += modv;
        }

        used[(size_t)val] = 1;
        sum_b += (seq_t)val;
    }

    return (sum_b - (seq_t)2) / ((seq_t)(n + 2));
}
