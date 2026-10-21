/*
 * A247801  Numbers in decimal representation with distinct digits, such that
 *          in Danish their digits are in alphabetic order.
 * https://oeis.org/A247801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word, fini, full
 * Formula:   a(n) is the nth smallest nonnegative integer whose decimal
 *            digits are distinct and in increasing alphabetic order according
 *            to Danish digit names. The sequence is sorted in increasing
 *            numeric value
 * Range:     u8 n <= 1, u16 n <= 55, u32 n <= 992, u64 n <= 992,
 *            u128 n <= 992, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Danish order of digits: 1,5,4,9,0,8,6,7,2,3 */
static const int orderDigits[10] = {1,5,4,9,0,8,6,7,2,3};

static seq_t seq_arr[1000];
static size_t seq_len = 0;
static bool seq_inited = false;

static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

static void init_sequence(void)
{
    size_t cnt = 0;
    /* iterate over all non‑empty subsets of the 10 digits */
    for (uint32_t mask = 1; mask < (1u << 10); ++mask)
    {
        int firstDigit = -1;
        /* find the leading digit according to the Danish order */
        for (int i = 0; i < 10; ++i)
        {
            int d = orderDigits[i];
            if (mask & (1u << d))
            {
                firstDigit = d;
                break;
            }
        }
        /* skip numbers with a leading zero and more than one digit */
        if (firstDigit == 0 && (mask != 1))
            continue;

        seq_t val = 0;
        for (int i = 0; i < 10; ++i)
        {
            int d = orderDigits[i];
            if (mask & (1u << d))
                val = val * 10 + d;
        }
        seq_arr[cnt++] = val;
    }
    seq_len = cnt;                 /* should be 992 */
    qsort(seq_arr, seq_len, sizeof(seq_t), cmp_seq_t);
    seq_inited = true;
}

seq_t a(idx_t n)
{
    if (!seq_inited)
        init_sequence();
    if (n == 0)          /* undefined for offset 1, but guard anyway */
        return 0;
    if (n > seq_len)     /* out of bounds – return 0 as a safe value */
        return 0;
    return seq_arr[n - 1];
}
