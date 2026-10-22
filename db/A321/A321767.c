/*
 * A321767  Reverse runs of nonzero digits in ternary expansion of n and
 *          convert back to decimal.
 * https://oeis.org/A321767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 244, u16 n <= 8927, u32 n <= 8927, u64 n <= 8927,
 *            u128 n <= 8927, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS A321767:  
   Reverse the runs of non‑zero digits in the ternary expansion of n
   and convert the resulting ternary number back to decimal. */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case */
    if (n == 0)
        return 0;

    /* ---- Step 1: Convert n to base‑3 digits (most significant first) ---- */
    /* 64 digits are enough for 64‑bit unsigned integers */
    uint8_t digits[64];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 3);
        tmp = tmp / 3;
    }
    /* Reverse to have most significant first */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* ---- Step 2: Split into runs of non‑zero digits and zero groups ---- */
    idx_t runs[64];      /* non‑zero runs */
    size_t zbs[64];      /* zeros between runs */
    size_t run_cnt = 0;
    size_t zb_cnt = 0;
    size_t trailing_zeros = 0;

    size_t pos = 0;
    while (pos < len) {
        /* Extract a non‑zero run */
        idx_t run_val = 0;
        while (pos < len && digits[pos] != 0) {
            run_val = run_val * (idx_t)3 + digits[pos];
            ++pos;
        }
        runs[run_cnt++] = run_val;

        /* Count zeros following this run */
        size_t zero_cnt = 0;
        while (pos < len && digits[pos] == 0) {
            ++zero_cnt;
            ++pos;
        }

        if (pos == len) {
            trailing_zeros = zero_cnt;
            break;
        } else {
            zbs[zb_cnt++] = zero_cnt;
        }
    }

    /* ---- Step 3: Reverse the order of non‑zero runs ---- */
    for (size_t i = 0; i < run_cnt / 2; ++i) {
        idx_t tmp_run = runs[i];
        runs[i] = runs[run_cnt - 1 - i];
        runs[run_cnt - 1 - i] = tmp_run;
    }
    /* The zero groups (zbs) stay in their original order; they are
       implicitly moved by the reversal of the runs list. */

    /* ---- Step 4: Re‑build the result from the reversed runs ---- */
    seq_t result = 0;

    for (size_t i = 0; i < run_cnt; ++i) {
        /* Convert the run value back to ternary digits and append them. */
        uint8_t r_digits[64];
        size_t rlen = 0;
        idx_t r = runs[i];
        do { /* run is at least 1 */
            r_digits[rlen++] = (uint8_t)(r % (idx_t)3);
            r = r / (idx_t)3;
        } while (r > 0);
        /* Reverse to most significant first */
        for (size_t t = 0; t < rlen / 2; ++t) {
            uint8_t tmp_d = r_digits[t];
            r_digits[t] = r_digits[rlen - 1 - t];
            r_digits[rlen - 1 - t] = tmp_d;
        }

        for (size_t t = 0; t < rlen; ++t) {
            result = result * (seq_t)3 + (seq_t)r_digits[t];
        }

        /* Append zero group if this is not the last run. */
        if (i < zb_cnt) {
            for (size_t z = 0; z < zbs[i]; ++z) {
                result = result * (seq_t)3;   /* zero digit */
            }
        }
    }

    /* Append the trailing zeros (if any) */
    for (size_t z = 0; z < trailing_zeros; ++z) {
        result = result * (seq_t)3;
    }

    return result;
}
