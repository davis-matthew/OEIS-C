/*
 * A323832  Start with n and repeatedly double it and apply the "repeatedly
 *          delete any run of identical digits" operation described in
 *          A323830; a(n) is the number of steps needed to reach one of 0, 1,
 *          or 5, or -1 if none of these three numbers is ever reached.
 * https://oeis.org/A323832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of steps needed to reach 0, 1, or 5 * by
 *            repeatedly doubling n and deleting all runs of identical *
 *            consecutive digits in its decimal representation. * * This
 *            implementation directly simulates the process
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_DIGITS 2048   /* maximum decimal digits handled per step */

/* Delete all runs of identical consecutive digits from the decimal
 * representation of x.  The process is repeated until no more runs
 * exist.  The routine works for any seq_t that supports the usual
 * arithmetic operators and comparison with 0. */
static seq_t delete_runs(seq_t x)
{
    int rev[MAX_DIGITS];
    int revlen;

    while (1) {
        /* Convert x to its decimal digits (least‑significant first). */
        seq_t tmp = x;
        revlen = 0;
        if (tmp == 0) {
            revlen = 0;
        } else {
            while (tmp != 0 && revlen < MAX_DIGITS) {
                rev[revlen++] = (int)(tmp % 10);
                tmp /= 10;
            }
        }

        /* Build an array with most‑significant digit first. */
        int digits[MAX_DIGITS];
        int len = 0;
        for (int i = revlen - 1; i >= 0; --i) {
            digits[len++] = rev[i];
        }

        /* Remove runs of the same digit. */
        int newdigits[MAX_DIGITS];
        int newlen = 0;
        bool anyRun = false;
        int i = 0;
        while (i < len) {
            int j = i + 1;
            while (j < len && digits[j] == digits[i]) {
                ++j;
            }
            if (j - i >= 2) {        /* run found */
                anyRun = true;
                i = j;               /* skip entire run */
            } else {                 /* keep the single digit */
                newdigits[newlen++] = digits[i];
                ++i;
            }
        }

        /* If no runs were removed, the number is stable. */
        if (!anyRun) {
            return x;
        }

        /* Construct the new number from the remaining digits. */
        seq_t y = 0;
        for (int k = 0; k < newlen; ++k) {
            y = y * 10 + (seq_t)newdigits[k];
        }
        x = y;                      /* repeat until stable */
    }
}

/* Compute a(n) as defined above. */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t steps = 0;

    /* If the starting value is already terminal, return 0 steps. */
    if (cur == 0 || cur == 1 || cur == 5) {
        return steps;
    }

    /* Iterate until we reach a terminal value. */
    while (1) {
        /* Apply the operation: double, then delete runs. */
        cur = delete_runs(cur * 2);
        ++steps;

        if (cur == 0 || cur == 1 || cur == 5) {
            return steps;
        }

        /* To guard against a pathological infinite loop (unlikely
           according to the conjecture), we limit the number of steps.
           If the limit is exceeded, return (seq_t)-1. */
        if (steps == (seq_t)UINT64_MAX) {
            return (seq_t)-1;
        }
    }
}
