/*
 * A273039  Numbers k such that the following process converges to zero:
 *          x(0)=k, x(i+1) = x(i) XOR ror(x(i)) XOR rol(x(i)); see the
 *          Comments section for details.
 * https://oeis.org/A273039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A273039: numbers k such that the iterative process
   x_{i+1} = x_i XOR ror(x_i) XOR rol(x_i) eventually reaches 0.
   The function generates the sequence by testing integers
   sequentially for convergence. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/bitlen_seq_t.h"

/* One iteration of the process:  f(x) = x XOR rol(x) XOR ror(x).  */
static inline seq_t f_xor_rotate(seq_t x)
{
    if (x == 0)
        return 0;

    /* number of bits needed to represent x */
    unsigned len = bitlen_seq_t(x);
    unsigned width = sizeof(seq_t) * 8;

    /* mask for the significant bits (length == width => all 1s) */
    seq_t mask;
    if (len == width)
        mask = ~((seq_t)0);
    else
        mask = (((seq_t)1 << len) - 1);

    /* rotate left and right on `len` bits */
    seq_t left  = ((x << 1) & mask) | (x >> (len - 1));
    seq_t right = (x >> 1) | ((x << (len - 1)) & mask);

    return x ^ left ^ right;
}

/* Test whether the process starting at `k` eventually reaches 0. */
static bool converges(seq_t k)
{
    if (k == 0)
        return true;

    seq_t tortoise = f_xor_rotate(k);
    seq_t hare = f_xor_rotate(f_xor_rotate(k));

    while (true) {
        if (tortoise == 0 || hare == 0)
            return true;
        if (tortoise == hare)
            return false;

        tortoise = f_xor_rotate(tortoise);
        hare = f_xor_rotate(f_xor_rotate(hare));
    }
}

/* Main interface required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, but handle gracefully */
        return 0;

    static seq_t *found = NULL;   /* sorted list of convergent k   */
    static size_t found_len = 0;  /* how many are stored          */
    static size_t found_cap = 0;  /* capacity of the array        */
    static idx_t last_checked = 0;   /* last integer examined       */

    /* Ensure that 0 is the first element (a(1)=0). */
    if (found_len == 0) {
        if (found_cap == 0) {
            found_cap = 4;
            found = (seq_t *)malloc(found_cap * sizeof(seq_t));
            if (!found) abort();
        }
        found[0] = (seq_t)0;
        found_len = 1;
        last_checked = 0;   /* next to check will be 1 */
    }

    /* Generate terms until we have at least `n` of them. */
    while (found_len < (size_t)n) {
        seq_t k = (seq_t)(last_checked + 1);
        if (converges(k)) {
            /* ensure capacity */
            if (found_len == found_cap) {
                size_t new_cap = found_cap * 2;
                seq_t *tmp = (seq_t *)realloc(found,
                                               new_cap * sizeof(seq_t));
                if (!tmp) abort();
                found = tmp;
                found_cap = new_cap;
            }
            found[found_len++] = k;
        }
        last_checked++;
    }

    return found[n - 1];
}
