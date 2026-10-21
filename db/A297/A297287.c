/*
 * A297287  Numbers whose base-15 digits have greater up-variation than
 *          down-variation; see Comments.
 * https://oeis.org/A297287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer whose base‑15 digits have
 * up‑variation (sum of decreases when moving from lower to higher
 * significance) greater than down‑variation (sum of increases).
 * This implements OEIS sequence A297287. */

#include <stdbool.h>
#include <stddef.h>

/* Return true if x has UV > DV in base 15. */
static bool good(seq_t x)
{
    /* No variation for single‑digit numbers. */
    if (x < (seq_t)15)
        return false;

    /* Extract base‑15 digits least significant first. */
    unsigned char dig[64];
    size_t len = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        dig[len++] = (unsigned char)(tmp % (seq_t)15);
        tmp        = tmp / (seq_t)15;
    }
    if (len <= 1)          /* already handled above, but keep for safety */
        return false;

    seq_t UV = 0, DV = 0;
    for (size_t i = 1; i < len; ++i) {
        unsigned int prev = dig[i - 1];  /* less significant digit */
        unsigned int cur  = dig[i];      /* more significant digit */
        if (cur > prev)
            DV += (seq_t)(cur - prev);  /* increase */
        else if (cur < prev)
            UV += (seq_t)(prev - cur);  /* decrease */
        /* equal digits contribute nothing */
    }
    return UV > DV;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* By definition, a(1) is the first good integer. */
    if (n == 0)
        return (seq_t)0;

    idx_t found = 0;     /* how many good numbers found */
    idx_t cand  = 1;     /* current candidate integer */

    while (found < n) {
        if (good((seq_t)cand))
            ++found;
        ++cand;
    }

    return (seq_t)(cand - 1);
}
