/*
 * A245691  Irregular triangle of Collatz like iteration, x -> 3x, then repeat
 *          (x -> ceiling(x/2) if divisible by 3, otherwise x -> 3x) while x
 *          != 6.
 * https://oeis.org/A245691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Defined by the irregular triangle of Collatz‑like iteration.
   For each starting integer m (offset 1), a row is produced:
   start with m, then multiply by 3, then while the current value is
   not 6 apply
   - if divisible by 3:  ceil(x/2) = (x+1)/2
   - otherwise:          x = 3*x
   All values of the row, including the initial m and the terminal 6,
   are appended to the infinite sequence.  a(n) returns the n‑th value
   of this concatenated sequence. */

#include <stdlib.h>

static seq_t *seq_buf  = NULL;   /* buffer holding generated terms   */
static size_t seq_len  = 0;      /* number of stored terms             */
static size_t seq_cap  = 0;      /* allocated capacity of buffer       */
static idx_t  next_start = 1;    /* next integer whose row to generate */

static void append_seq(seq_t v)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 128;
#if defined(__cplusplus)
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < seq_len; ++i)
            tmp[i] = seq_buf[i];
        delete[] seq_buf;
        seq_buf = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_buf = tmp;
#endif
        seq_cap = new_cap;
    }
    seq_buf[seq_len++] = v;
}

seq_t a(idx_t n)
{
    while (seq_len < (size_t)n) {
        idx_t m = next_start++;
        seq_t x = (seq_t)m;
        append_seq(x);

        /* First mandatory multiply by 3. */
        x = x * (seq_t)3;
        append_seq(x);

        /* Iterate until x becomes 6. */
        while (x != (seq_t)6) {
            if ((x % (seq_t)3) == 0)
                x = (x + (seq_t)1) / (seq_t)2;  /* ceil(x/2) */
            else
                x = x * (seq_t)3;
            append_seq(x);
        }
    }
    return seq_buf[(size_t)n - 1];
}
