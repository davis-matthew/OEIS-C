/*
 * A292439  List of numbers k whose consecutive digits increase or decrease by
 *          d-1, where d is the number of digits in k.
 * https://oeis.org/A292439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 73, u32 n <= 128, u64 n <= 131,
 *            u128 n <= 131, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates OEIS A292439:
   List of numbers k whose consecutive digits increase or decrease by d‑1,
   where d is the number of digits in k.
   The sequence is finite (131 terms). */

/* Types seq_t and idx_t are provided by the build environment. */

#include <stdlib.h>
#include <stdbool.h>

/* local storage for the sequence, filled once */
static seq_t seq_buf[200];
static size_t seq_len = 0;
static bool digits_done = false;

/* helper: compare two seq_t values for qsort */
static int seq_cmp(const void *p, const void *q)
{
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* recursive generator for a fixed number of digits d
   diff = d-1, depth counts digits already formed, last is the last digit
   cur is the numeric value of the digits formed so far                 */
static void gen_rec(int d, int depth, int last, int diff, seq_t cur)
{
    if (depth == d) {
        seq_buf[seq_len++] = cur;
        return;
    }
    int nxt = last + diff;
    if (nxt <= 9)
        gen_rec(d, depth + 1, nxt, diff, cur * (seq_t)10 + (seq_t)nxt);
    nxt = last - diff;
    if (nxt >= 0)
        gen_rec(d, depth + 1, nxt, diff, cur * (seq_t)10 + (seq_t)nxt);
}

static void build_sequence(void)
{
    if (digits_done)
        return;

    seq_len = 0;

    /* d = 1 : all digits 0..9 (no adjacency condition) */
    for (int d = 0; d <= 9; ++d)
        seq_buf[seq_len++] = (seq_t)d;

    /* d >= 2 up to 10 (since d-1 must be <=9) */
    for (int d = 2; d <= 10; ++d) {
        int diff = d - 1;
        for (int first = 1; first <= 9; ++first) {
            gen_rec(d, 1, first, diff, (seq_t)first);
        }
    }

    /* sort the list */
    qsort(seq_buf, seq_len, sizeof(seq_t), seq_cmp);

    digits_done = true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    build_sequence();

    if ((idx_t)seq_len < n)
        return 0;
    return seq_buf[(size_t)n - 1];
}
