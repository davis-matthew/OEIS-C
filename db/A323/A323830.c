/*
 * A323830  a(0) = 1; thereafter a(n) is obtained by doubling a(n-1) and
 *          repeatedly deleting any string of identical digits.
 * https://oeis.org/A323830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A323830
   a(0) = 1;
   For n ≥ 1, a(n) = A321801(2 * a(n‑1)),
   where A321801 deletes every maximal run of identical decimal
   digits until none remain.  The sequence is periodic with period 20. */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */

/* Delete a single maximal run of equal digits from x.
   Runs of length 1 are kept, runs of length ≥2 are removed. */
static seq_t delete_runs_once(seq_t x)
{
    /* Extract decimal digits of x (least‑significant first). */
    uint8_t digits[200];
    size_t len = 0;
    seq_t tmp = x;

    if (tmp == 0) {
        digits[len++] = 0;
    } else {
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    /* Build kept digits from most‑significant to least. */
    uint8_t kept[200];
    size_t keptlen = 0;

    for (size_t i = len; i > 0; ) {
        size_t pos = i - 1;          /* index of current digit */
        size_t start = pos;

        /* Extend backwards while digits are equal. */
        while (start > 0 && digits[start - 1] == digits[pos]) {
            --start;
        }

        size_t run_len = i - start;
        if (run_len == 1) {
            /* Keep this single digit. */
            kept[keptlen++] = digits[pos];
        }
        i = start;   /* continue with preceding run */
    }

    /* Reconstruct number from kept digits (most‑significant first). */
    seq_t res = 0;
    for (size_t k = 0; k < keptlen; ++k) {
        res = res * 10 + kept[k];
    }
    return res;
}

/* Repeatedly delete runs until the number stabilises. */
static seq_t delete_runs_full(seq_t x)
{
    seq_t prev;
    do {
        prev = x;
        x = delete_runs_once(x);
    } while (x != prev);
    return x;
}

/* External function implementing the sequence. */
seq_t a(idx_t n)
{
    /* Cache for the first period (length 20). */
    static seq_t period[20];
    static bool inited = false;

    if (!inited) {
        period[0] = 1;
        for (idx_t i = 1; i < 20; ++i) {
            seq_t doubled = period[i - 1] * (seq_t)2;
            period[i] = delete_runs_full(doubled);
        }
        inited = true;
    }

    return period[(size_t)(n % 20)];
}
