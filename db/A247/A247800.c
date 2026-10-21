/*
 * A247800  Numbers in decimal representation with distinct digits, such that
 *          in Czech their digits are in alphabetic order.
 * https://oeis.org/A247800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word, fini, full
 * Formula:   This function generates A247800 by enumerating all non‑empty
 *            subsets of decimal digits 0–9, constructing the number whose
 *            digits appear in the Czech alphabetic order
 *            (4,9,2,1,0,8,5,7,6,3), excluding subsets that would produce a
 *            leading zero (except the single digit 0 itself), and then
 *            sorting the...
 * Range:     u8 n <= 1, u16 n <= 123, u32 n <= 990, u64 n <= 992,
 *            u128 n <= 992, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const int order[10] = {4,9,2,1,0,8,5,7,6,3};
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;

static void init_seq(void)
{
    const size_t cap = 1023;                      /* 2^10 – 1 subsets */
    seq_arr = (seq_t *)malloc(cap * sizeof(seq_t));
    seq_len = 0;

    const uint16_t mask0    = 1u << 0;                                 /* digit 0 */
    const uint16_t mask0124 = (1u << 4) | (1u << 9) | (1u << 2) | (1u << 1); /* digits 4,9,2,1 */
    const uint16_t full     = (1u << 10) - 1;                           /* all digits */

    for (uint16_t mask = 1; mask <= full; ++mask)
    {
        /* Exclude subsets that would start with a leading zero (except {0}). */
        if ( (mask & mask0) && ((mask & mask0124) == 0) && (mask != mask0) )
            continue;

        seq_t value = 0;
        for (int k = 0; k < 10; ++k)
        {
            int d = order[k];
            if (mask & (1u << d))
                value = value * 10 + d;
        }
        seq_arr[seq_len++] = value;
    }

    /* Sort numerically.  seq_len ≤ 992, insertion sort is efficient enough. */
    for (size_t i = 1; i < seq_len; ++i)
    {
        seq_t key = seq_arr[i];
        size_t j = i;
        while (j > 0 && seq_arr[j - 1] > key)
        {
            seq_arr[j] = seq_arr[j - 1];
            --j;
        }
        seq_arr[j] = key;
    }
}

seq_t a(idx_t n)
{
    if (!seq_arr)
        init_seq();

    /* The offset for OEIS is 1; return 0 for out‑of‑range requests. */
    if (n == 0 || (size_t)n > seq_len)
        return 0;
    return seq_arr[n - 1];
}
