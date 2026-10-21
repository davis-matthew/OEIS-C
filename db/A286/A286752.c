/*
 * A286752  {010010->null}-transform of the infinite Fibonacci word A003849.
 * https://oeis.org/A286752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A286752: delete every non‑overlapping occurrence of 010010 from
   the infinite Fibonacci word (A003849). */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* static data */
static uint8_t *fib_prefix = NULL;
static size_t   fib_len    = 0;   /* number of bits stored in fib_prefix */
static idx_t    input_pos   = 0;   /* next input index to process */

static seq_t *output_arr = NULL;
static size_t output_len = 0;     /* number of bits already produced */
static size_t output_cap = 0;     /* capacity of output_arr */

/* Rebuild fib_prefix so it contains at least 'target' bits of the
   Fibonacci word.  The word is constructed iteratively:
   w1 = 0, w2 = 01, w(k+1) = w(k) w(k-1). */
static void ensure_fib_prefix (size_t target)
{
    if (target <= fib_len)
        return;   /* already long enough */

    /* build the Fibonacci word until length >= target */
    size_t len1 = 1, len2 = 2;
    uint8_t *w1 = (uint8_t *)malloc(len1);   /* w1 = 0 */
    uint8_t *w2 = (uint8_t *)malloc(len2);   /* w2 = 01 */
    w1[0] = 0;
    w2[0] = 0;  w2[1] = 1;

    if (len2 < target) {
        while (len2 < target) {
            size_t len3 = len2 + len1;
            uint8_t *w3 = (uint8_t *)malloc(len3);
            /* w3 = w2 followed by w1 */
            for (size_t i = 0; i < len2; ++i) w3[i] = w2[i];
            for (size_t i = 0; i < len1; ++i) w3[len2 + i] = w1[i];

            /* shift words */
            uint8_t *old_w1 = w1;
            w1 = w2;   len1 = len2;
            w2 = w3;   len2 = len3;
            free(old_w1);
        }
        /* w1 no longer needed */
        free(w1);
    } else {
        free(w1);   /* w2 already long enough */
    }

    /* copy first 'target' bits to fib_prefix */
    uint8_t *final = (uint8_t *)malloc(target);
    for (size_t i = 0; i < target; ++i)
        final[i] = w2[i];
    free(w2);
    free(fib_prefix);
    fib_prefix = final;
    fib_len    = target;
}

/* Ensure output_arr can hold at least 'needed' seq_t elements. */
static void ensure_output_capacity (size_t needed)
{
    if (output_cap >= needed)
        return;
    size_t new_cap = output_cap ? output_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(output_arr, new_cap * sizeof(seq_t));
    if (!new_buf) exit(1);               /* out of memory */
    output_arr = new_buf;
    output_cap = new_cap;
}

/* Return the nth term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* not defined, but harmless */

    /* generate output until we have n bits */
    while (output_len < n) {
        /* Ensure we have enough input bits to test a 6‑bit window. */
        if (fib_len < input_pos + 6) {
            size_t new_target = fib_len ? fib_len << 1 : 32;
            if (new_target < input_pos + 6)
                new_target = input_pos + 6;
            ensure_fib_prefix(new_target);
        }

        /* If a full 6‑bit window exists, check for the pattern 010010. */
        if (input_pos + 5 < fib_len) {
            if (fib_prefix[input_pos]     == 0 &&
                fib_prefix[input_pos + 1] == 1 &&
                fib_prefix[input_pos + 2] == 0 &&
                fib_prefix[input_pos + 3] == 0 &&
                fib_prefix[input_pos + 4] == 1 &&
                fib_prefix[input_pos + 5] == 0) {
                input_pos += 6;            /* delete the pattern */
                continue;
            }
            /* otherwise output the current bit */
            ensure_output_capacity(output_len + 1);
            output_arr[output_len++] = (seq_t)fib_prefix[input_pos++];
        } else {
            /* not enough input yet; extend the Fibonacci prefix. */
            size_t new_target = fib_len ? fib_len << 1 : 32;
            if (new_target < input_pos + 6)
                new_target = input_pos + 6;
            ensure_fib_prefix(new_target);
        }
    }

    return output_arr[n - 1];
}
