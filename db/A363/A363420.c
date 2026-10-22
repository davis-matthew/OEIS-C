/*
 * A363420  Let D be the largest digit of a(n) + a(n+1). The successive Ds
 *          reproduce the successive digits of the sequence. By construction,
 *          we do not accept any zero in the sequence. This is the
 *          lexicographically earliest sequence of distinct terms > 0 with
 *          this property.
 * https://oeis.org/A363420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1; for n≥1, a(n+1) is the smallest positive integer * that
 *            (i) contains no zero digit, * (ii) is distinct from all earlier
 *            a(k), * (iii) satisfies: largest digit of (a(n)+a(n+1)) * equals
 *            the n‑th digit of the decimal concatenation * a(1)a(2)…a(n). *
 *            The implementation constructs the sequenc...
 * Range:     u8 n <= 59, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/*--- helper functions --------------------------------------------------*/
static bool has_zero_digit(seq_t x)
{
    while (x != 0)
    {
        if (x % 10 == 0)
            return true;
        x /= 10;
    }
    return false;
}

static unsigned largest_digit(seq_t x)
{
    unsigned max_d = 0;
    while (x != 0)
    {
        unsigned d = (unsigned)(x % 10);
        if (d > max_d)
            max_d = d;
        x /= 10;
    }
    return max_d;
}

/*--- dynamic arrays for the sequence and its concatenated digits ----*/
static seq_t  *sequence  = NULL;
static size_t  seq_len   = 0;   /* number of terms stored          */
static size_t  seq_cap   = 0;   /* capacity of sequence array      */

static unsigned char *digits  = NULL;
static size_t         digits_len = 0;   /* number of digits stored        */
static size_t         digits_cap = 0;   /* capacity of digits array       */

static void grow_seq(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(sequence, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    sequence = tmp;
    seq_cap   = new_cap;
}

static void grow_digits(size_t needed)
{
    if (needed <= digits_cap) return;
    size_t new_cap = digits_cap ? digits_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    unsigned char *tmp = (unsigned char *)realloc(digits, new_cap * sizeof(unsigned char));
    if (!tmp) abort();
    digits   = tmp;
    digits_cap = new_cap;
}

/* Append the decimal digits of v (in normal order) to the digits array. */
static void append_digits(seq_t v)
{
    /* The largest number of decimal digits of a 128‑bit value is 39.
       64 is more than enough for any supported build. */
    unsigned char tmp[64];
    size_t cnt = 0;
    while (v != 0)
    {
        tmp[cnt++] = (unsigned char)(v % 10);
        v /= 10;
    }
    /* v=0 never occurs because a(n) > 0, but keep the logic for completeness */
    if (cnt == 0) { tmp[cnt++] = 0; }

    for (size_t i = 0; i < cnt; ++i)
    {
        grow_digits(digits_len + 1);
        digits[digits_len++] = tmp[cnt - 1 - i];
    }
}

/*--- public sequence function ----------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, but harmless */

    /* Build the sequence up to index n . */
    while (seq_len < (size_t)n)
    {
        /* First term is fixed. */
        if (seq_len == 0)
        {
            seq_t first = 1;
            grow_seq(1);
            sequence[seq_len++] = first;
            append_digits(first);
            continue;
        }

        /* Determine a(seq_len+1). */
        unsigned target = digits[seq_len - 1];   /* nth digit of concatenation */
        seq_t   prev    = sequence[seq_len - 1];

        seq_t cand = 1;
        for (;; ++cand)
        {
            /* Rule (i): no zero digit. */
            if (has_zero_digit(cand))
                continue;

            /* Rule (ii): distinctness check. */
            bool used = false;
            for (size_t i = 0; i < seq_len; ++i)
            {
                if (sequence[i] == cand)
                {
                    used = true;
                    break;
                }
            }
            if (used) continue;

            /* Rule (iii): largest digit of sum equals target. */
            seq_t sum = prev + cand;
            if (largest_digit(sum) == target)
                break;
        }

        grow_seq(seq_len + 1);
        sequence[seq_len++] = cand;
        append_digits(cand);
    }

    return sequence[n - 1];
}
