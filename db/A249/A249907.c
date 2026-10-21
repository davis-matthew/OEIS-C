/*
 * A249907  Smallest positive integer k such that k contains all possible
 *          pairs of digits when represented in base b = n >= 2.
 * https://oeis.org/A249907
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest integer that contains all digit pairs in base n
   (de Bruijn sequence of order 2). */
#include <stdlib.h>
#include <stdint.h>

/* Hierholzer's algorithm for the de Bruijn graph of order 2 over the
   alphabet {0,…,n-1}.  The traversal starts at vertex 1, so the
   resulting digit string begins with 1 and contains every pair exactly
   once, and its value in base n is returned. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                           /* offset 2 */

    /* Number of edges: n^2; length of digit string: n^2 + 1 */
    size_t nb  = (size_t)n * (size_t)n;
    size_t len = nb + 1;

    /* next[v] = next unused outgoing edge from vertex v (0…n-1) */
    uint32_t *next = (uint32_t *)calloc((size_t)n, sizeof(uint32_t));
    if (!next)
        return 0;

    /* stack for Hierholzer's algorithm */
    uint32_t *stack = (uint32_t *)calloc(len, sizeof(uint32_t));
    /* output array (reversed order before final reversal) */
    uint32_t *rev   = (uint32_t *)calloc(len, sizeof(uint32_t));
    if (!stack || !rev) {
        free(next); free(stack); free(rev);
        return 0;
    }

    size_t top = 0;                /* stack top index (inclusive) */
    stack[0] = (uint32_t)1;        /* start vertex = 1 */
    size_t outp = 0;               /* position in rev */

    while (top < len) {
        uint32_t v = stack[top];
        if (next[v] < (uint32_t)n) {
            uint32_t u = next[v]++;    /* use next unused edge v → u   */
            stack[++top] = u;
        } else {
            rev[outp++] = v;            /* record vertex on backtrack   */
            top--;                      /* pop stack                     */
        }
    }

    /* rev now contains the circuit in reverse order; length should equal len */
    if (outp != len) {
        free(next); free(stack); free(rev);
        return 0;
    }

    /* Reverse rev in place to obtain the correct digit order */
    for (size_t i = 0, j = len - 1; i < j; ++i, --j) {
        uint32_t tmp = rev[i];
        rev[i] = rev[j];
        rev[j] = tmp;
    }

    /* Convert the digit sequence into an integer in base n */
    seq_t value = 0;
    for (size_t i = 0; i < len; ++i)
        value = value * (seq_t)n + (seq_t)rev[i];

    free(next);
    free(stack);
    free(rev);
    return value;
}
