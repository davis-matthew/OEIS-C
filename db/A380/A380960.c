/*
 * A380960  Sum of n and the n-th bit of the infinite Fibonacci word.
 * https://oeis.org/A380960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + A003849(n) (A003849(n) = nth bit of the Fibonacci
 *            word)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint8_t *fib_bits = NULL;
static idx_t fib_bits_len = 0;

/* Expand the stored Fibonacci word until its length exceeds n */
static void ensure_bits(idx_t n)
{
    /* initialise if necessary */
    if (fib_bits_len == 0) {
        fib_bits_len = 1;
        fib_bits = (uint8_t *)malloc(sizeof(uint8_t));
        fib_bits[0] = 0;                 /* first symbol of the word */
    }

    while (fib_bits_len <= n) {
        /* count zeros in the current word */
        idx_t zeros = 0;
        for (idx_t i = 0; i < fib_bits_len; ++i)
            if (fib_bits[i] == 0)
                ++zeros;

        /* new length after applying the morphism 0→01, 1→0 */
        idx_t new_len = fib_bits_len + zeros;
        uint8_t *new_arr = (uint8_t *)malloc(new_len * sizeof(uint8_t));
        idx_t pos = 0;
        for (idx_t i = 0; i < fib_bits_len; ++i) {
            if (fib_bits[i] == 0) {
                new_arr[pos++] = 0;
                new_arr[pos++] = 1;
            } else {
                new_arr[pos++] = 0;
            }
        }

        free(fib_bits);
        fib_bits = new_arr;
        fib_bits_len = new_len;
    }
}

seq_t a(idx_t n)
{
    ensure_bits(n);
    seq_t bit = (seq_t)fib_bits[n];
    return ((seq_t)n) + bit;
}
