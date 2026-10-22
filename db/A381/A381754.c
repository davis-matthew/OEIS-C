/*
 * A381754  Numbers k such that k and 3*k have the same number of zeros in
 *          their binary expansions.
 * https://oeis.org/A381754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 3894, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A381754: numbers k such that k and 3*k have the same number of zeros
   in their binary representations.  The implementation keeps a
   static cache and extends it up to the requested index on demand. */

#include <stdlib.h>

static size_t count_zeros(seq_t x)
{
    if (x == (seq_t)0) return 1;
    size_t zeros = 0;
    while (x) {
        if (!(x & (seq_t)1)) ++zeros;
        x >>= 1;
    }
    return zeros;
}

static int equal_zero_count(seq_t k)
{
    return count_zeros(k) == count_zeros(k * (seq_t)3);
}

seq_t a(idx_t n)
{
    /* Handle the case that the caller passes n == 0, though the
       sequence is defined for n >= 1.  Returning 0 is harmless. */
    if (n == 0) return (seq_t)0;

    /* Static state for memoisation. */
    static seq_t *buf   = NULL;   /* array of found terms             */
    static size_t size   = 0;     /* number of stored terms           */
    static size_t cap    = 0;     /* current capacity of the array    */
    static seq_t next_k  = 0;     /* next candidate k to test         */

    /* Initialise the array on first call. */
    if (buf == NULL) {
        cap = 128;
        buf = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!buf) return (seq_t)0; /* out‑of‑memory fallback */
    }

    /* Grow the cache until it contains at least n terms. */
    while (size < (size_t)n) {
        if (equal_zero_count(next_k)) {
            if (size >= cap) {
                cap <<= 1;                 /* double the capacity      */
                seq_t *tmp = (seq_t*)realloc(buf, cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* out‑of‑memory fallback */
                buf = tmp;
            }
            buf[size++] = next_k;
        }
        ++next_k;
    }

    return buf[n-1];
}
