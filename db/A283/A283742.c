/*
 * A283742  Numbers with digit sum 11 that are multiples of 11.
 * https://oeis.org/A283742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th smallest decimal integer whose digit sum is 11
 *            and whose digits in even positions (counting from the left,
 *            1‑based) are all zero. The sequence consists of all numbers of
 *            the form d0 0 d1 0 d2 0 ... such that d0 + d1 + d2 + ... = 11
 *            and d0 > 0. Numbers are sorted increasingly, whic...
 * Range:     u8 n <= 1, u16 n <= 65, u32 n <= 2475, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq = NULL;            /* array of sequence values          */
static size_t seq_len = 0;           /* number of values stored           */
static size_t seq_cap = 0;           /* allocated capacity                */
static int next_L = 3;               /* next length of digits to generate */

static seq_t pow10[256];
static bool pow10_init = false;

/* initialise the table of powers of 10; pow10[i] = 10^i */
static void init_pow10(void)
{
    if (!pow10_init) {
        pow10[0] = (seq_t)1;
        for (int i = 1; i < 256; ++i)
            pow10[i] = pow10[i - 1] * (seq_t)10;
        pow10_init = true;
    }
}

/* ensure the dynamic array can hold 'needed' elements */
static void ensure_cap(size_t needed)
{
    if (needed > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 128;
        while (new_cap < needed) new_cap *= 2;
        seq_t *new_arr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq = new_arr;
        seq_cap = new_cap;
    }
}

/* globals used by the recursive generator */
static int gen_m;          /* number of odd positions for current length */
static int gen_L_val;      /* current length of the number (digits) */

/* recursive generation of one length; pos counts odd positions from left */
static void gen_rec(int pos, int sum, seq_t cur)
{
    if (pos == gen_m) {
        if (sum == 0) {
            ensure_cap(seq_len + 1);
            seq[seq_len++] = cur;
        }
        return;
    }
    int min_d = (pos == 0) ? 1 : 0;           /* leading digit must be >0 */
    int max_d = sum <= 9 ? sum : 9;
    seq_t weight = pow10[gen_L_val - 1 - 2 * pos];
    for (int d = min_d; d <= max_d; ++d) {
        gen_rec(pos + 1, sum - d, cur + (seq_t)d * weight);
    }
}

/* generate all numbers of length 'L' and append them to the global array */
static void generate_for_length(int L)
{
    gen_L_val = L;
    gen_m = (L + 1) / 2;          /* number of odd positions */
    gen_rec(0, 11, (seq_t)0);
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */
    init_pow10();
    while (seq_len < n) {
        generate_for_length(next_L);
        ++next_L;
    }
    return seq[n - 1];
}
