/*
 * A307096  Positive integers m such that for any positive integer k the last
 *          k bits of the binary expansion of m is not a multiple of 3.
 * https://oeis.org/A307096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) computed by iteratively constructing the sequence from the
 *            recurrence that an integer m is in the sequence iff m is not
 *            divisible by 3 and (m - 2^{⟨m⟩}) is in the sequence, where ⟨m⟩
 *            is ⌈log₂(m+1)⌉. The sequence starts a(1)=1
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static storage for the generated sequence */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static idx_t cur_bitlen = 0;    /* current maximum bit length in seq_arr */
static seq_t shift = 0;         /* 2^{cur_bitlen} for the next level */

/* Ensure that seq_arr has enough capacity to add 'need' more elements */
static void ensure_capacity(size_t need)
{
    if (seq_len + need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (seq_len + need > new_cap)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_arr = tmp;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, so n==0 is undefined, return 0 safely */
        return 0;

    /* First call initialises the data */
    if (seq_arr == NULL)
    {
        seq_cap = 16;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 1;
        seq_arr[0] = (seq_t)1;          /* a(1) = 1 */
        cur_bitlen = 1;                 /* numbers with 1 bit already present */
        shift = ((seq_t)1) << cur_bitlen; /* ready shift for next bit length (2^1) */
    }

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n)
    {
        size_t prev_len = seq_len;      /* numbers with bit length <= cur_bitlen */

        for (size_t i = 0; i < prev_len; ++i)
        {
            seq_t y = shift + seq_arr[i];                /* candidate number  */
            if ((y % (seq_t)3) != 0)                     /* not divisible by 3 */
            {
                ensure_capacity(1);
                seq_arr[seq_len++] = y;                 /* append to list */
            }
        }

        shift <<= 1;               /* advance to 2^{cur_bitlen+1} */
        ++cur_bitlen;
    }

    return seq_arr[n - 1];
}
