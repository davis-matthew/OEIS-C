/*
 * A343150  Reverse the order of all but the most significant bits in the
 *          minimal Fibonacci expansion of n.
 * https://oeis.org/A343150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A343150: a(n) = value obtained by taking the Zeckendorf (minimal Fibonacci) expansion of n (in little‑endian bit order),
   keeping the most significant digit fixed and reversing all lower digits, then interpreting the result again as a Fibonacci expansion. */

#include <stdlib.h>

/* Automatic in‑place Fibonacci table for Zeckendorf expansions */
static seq_t *fib_table = NULL;
static idx_t fib_len = 0;

/* Ensure that fib_table contains Fibonacci numbers up to the
   smallest index `k` with fib[k] > n.  fib[0] = fib[1] = 1. */
static void ensure_fib(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (!fib_table) {
        fib_len = 2;
        fib_table = (seq_t *)malloc(2 * sizeof(seq_t));
        fib_table[0] = 1;
        fib_table[1] = 1;
    }
    /* Grow while the last element is <= n */
    while (fib_len > 0 && fib_table[fib_len-1] <= nn) {
        size_t new_len = fib_len + 1;
        seq_t *tmp = (seq_t *)realloc(fib_table, new_len * sizeof(seq_t));
        fib_table = tmp;
        fib_table[new_len-1] = fib_table[new_len-2] + fib_table[new_len-3];
        fib_len = (idx_t)new_len;
    }
}

seq_t a(idx_t n)
{
    ensure_fib(n);

    /* msb_index = highest Fibonacci index that fits in n */
    idx_t msb_index = fib_len - 2;          /* fib[fib_len-1] > n */

    /* Build the Zeckendorf binary bits in little‑endian order */
    unsigned char *bits = (unsigned char *)calloc(fib_len, sizeof(unsigned char));
    seq_t rem = (seq_t)n;
    for (size_t i = fib_len; i > 0; --i) {
        size_t k = i - 1;
        if (fib_table[k] <= rem) {
            bits[k] = 1;
            rem -= fib_table[k];
        }
    }

    /* The result keeps the most significant bit and reverses the lower bits */
    seq_t res = fib_table[msb_index];
    for (idx_t i = 0; i < msb_index; ++i) {
        if (bits[msb_index - 1 - i]) {
            res += fib_table[i];
        }
    }

    free(bits);
    return res;
}
