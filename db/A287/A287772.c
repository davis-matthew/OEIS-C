/*
 * A287772  {0->1, 1->00}-transform of the infinite Fibonacci word A003849.
 * https://oeis.org/A287772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2953, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS A287772, the
 * {0→1, 1→00}–transform of the Fibonacci word (offset 1). 
 * The algorithm builds the transformed length sequence
 * T[1] = 1, T[2] = 3, T[i] = T[i‑1] + T[i‑2] for i≥3,
 * then recursively descends to locate the symbol at position
 * n by using the decomposition of the Fibonacci word. */

#include <stdlib.h>
#include "../utils/get_symbol.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache of transformed lengths */
    static seq_t *T   = NULL;   /* indices: 0 unused, 1,2,... */
    static int    Tlen = 0;     /* highest index currently stored */

    seq_t N = (seq_t)n;         /* work with seq_t for compares */

    /* Initialise T on first call */
    if (T == NULL) {
        T = (seq_t *)malloc(3 * sizeof(seq_t)); /* indices 0,1,2 */
        T[0] = (seq_t)0;          /* unused */
        T[1] = (seq_t)1;          /* length of transform w1 */
        T[2] = (seq_t)3;          /* length of transform w2 */
        Tlen = 2;
    }

    /* Grow T until it covers position N */
    while (N > T[Tlen]) {
        seq_t next = T[Tlen] + T[Tlen - 1];
        T = (seq_t *)realloc(T, (Tlen + 2) * sizeof(seq_t));
        T[Tlen + 1] = next;
        Tlen++;
    }

    return get_symbol(N, Tlen, T);
}
