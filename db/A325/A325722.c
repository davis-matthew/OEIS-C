/*
 * A325722  Start the sequence with a(1) = 1 and read the digits one by one
 *          from there. The sequence is always extended with the product d*k,
 *          d being the digit read and k the number of d digits present up to
 *          the point of the d digit taken.
 * https://oeis.org/A325722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 330, u16 n <= 5001, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1.  For each decimal digit of the sequence, read the
 * digits of the already generated terms from left to right.  If the current
 * digit is d and it is the k‑th occurrence of d among all digits read so
 * far (including this one), append the product d*k as the next term. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/pow10.h"    /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h" /* int digit_len(seq_t x); */

static seq_t *seq_cache = NULL;      /* array of the sequence values   */
static size_t seq_cap   = 0;         /* allocated capacity              */
static size_t seq_len   = 0;         /* number of valid terms in cache  */
static size_t process_idx = 0;       /* index of next term to process   */
static uint64_t digit_counts[10] = {0};

/* Ensure that the cache can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* Process the term at index 'idx'; read its decimal digits and append the
 * corresponding products to the sequence. */
static void process_term(size_t idx)
{
    seq_t term = seq_cache[idx];

    /* Special case: the digit 0 (term == 0) is a single digit. */
    if (term == (seq_t)0) {
        digit_counts[0]++;
        seq_t next = (seq_t)0;   /* 0 * k = 0 */
        ensure_capacity(seq_len + 1);
        seq_cache[seq_len++] = next;
        return;
    }

    int ndig = digit_len(term);            /* number of decimal digits */
    seq_t base = pow10((idx_t)(ndig - 1)); /* 10^(ndig-1) */
    seq_t rem  = term;

    for (int i = 0; i < ndig; ++i) {
        int d = (int)(rem / base);
        rem %= base;
        base /= 10;

        digit_counts[d]++;               /* this is the k‑th d */
        seq_t next = (seq_t)d * (seq_t)digit_counts[d];
        ensure_capacity(seq_len + 1);
        seq_cache[seq_len++] = next;
    }
}

/* Generate the sequence until at least 'target' terms are available. */
static void generate_until(idx_t target)
{
    while (seq_len < target) {
        process_term(process_idx);
        ++process_idx;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */

    /* One‑time initialization of static state. */
    if (seq_len == 0) {
        seq_cap = 16;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_cache[0] = (seq_t)1;          /* a(1) = 1 */
        seq_len   = 1;
        process_idx = 0;
        memset(digit_counts, 0, sizeof(digit_counts));
    }

    if ((idx_t)seq_len < n) {
        generate_until(n);
    }

    return seq_cache[n - 1];
}
