/*
 * A340125  Numbers whose sum of even digits and sum of odd digits are equal
 *          and whose digits are in nondecreasing order.
 * https://oeis.org/A340125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 58, u32 n <= 1532, u64 n <= 9919,
 *            u128 n <= 9919, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: numbers with non‑decreasing decimal digits where the
 * sum of even digits equals the sum of odd digits.  Sequence terms are
 * generated on demand and cached; the function works for any order of
 * queries. */
#include <stdlib.h>
#include <stdbool.h>

/* static storage for generated sequence */
static seq_t *seq = NULL;
static size_t seq_size = 0;   /* number of terms stored   */
static size_t seq_cap  = 0;   /* allocated capacity       */
static size_t max_len_generated = 0;   /* longest digit length generated */

/* ensure seq has at least *need* capacity */
static void ensure_cap(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 4;
    if (new_cap < need)
        new_cap = need;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq)
        abort(); /* out of memory */
    seq_cap = new_cap;
}

/* recursive generator for numbers of a given length L.
 * pos : current position (0‑based), cur_val : value built so far,
 * sum_even / sum_odd : accumulated sums of even / odd digits. */
static void gen_len(int pos, int L, int prev_digit,
                    seq_t cur_val,
                    seq_t sum_even, seq_t sum_odd)
{
    if (pos == L) {
        if (sum_even == sum_odd) {
            ensure_cap(seq_size + 1);
            seq[seq_size++] = cur_val;
        }
        return;
    }
    for (int d = prev_digit; d <= 9; ++d) {
        seq_t nv = cur_val * 10 + d;
        seq_t ne = sum_even, no = sum_odd;
        if (d & 1)
            no += d;
        else
            ne += d;
        gen_len(pos + 1, L, d, nv, ne, no);
    }
}

/* generate sequence at least up to n terms. */
static void generate_until(idx_t n)
{
    if (seq_size >= (size_t)n)
        return;

    /* include 0 as the first term once */
    if (seq_size == 0) {
        ensure_cap(1);
        seq[seq_size++] = (seq_t)0;
    }

    for (size_t L = max_len_generated + 1; seq_size < (size_t)n; ++L) {
        max_len_generated = L;
        gen_len(0, (int)L, 1, 0, 0, 0);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* undefined for offset 1 */
    generate_until(n);
    return seq[n - 1];                      /* zero‑based indexing */
}
